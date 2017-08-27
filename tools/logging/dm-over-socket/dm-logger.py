#!/usr/bin/python
import socket
import SocketServer
import sys
import os

outfile = None
sqf = None
VERSION_LEN = 3
ASYNC_HDLC_FLAG = '\x7e'
ip_version = 6
# 100MB
max_filesize = 100 * 1024 * 1024
# 50K
#max_filesize = 50 * 1024
rotate_number = 0
filepath = None

def RotateFile(pfile):
    global rotate_number
    global outfile
    #print 'rotate file'
    pfile.flush()
    pfile.close()
    filename, file_extension = os.path.splitext(filepath)
    newfilename = filename + ".%03d" % rotate_number + file_extension
    if os.path.exists(newfilename) == True:
        print 'Remove Exist File %s'% newfilename
        os.remove(newfilename)
    rotate_number += 1
    os.rename(filepath,newfilename);
    outfile = open(filepath,"wb")

def RotateLastFile(pfile):
    global rotate_number
    global outfile
    #print 'rotate file'
    pfile.flush()
    pfile.close()
    filename, file_extension = os.path.splitext(filepath)
    newfilename = filename + ".%03d" % rotate_number + file_extension
    if os.path.exists(newfilename) == True:
        print 'Remove Exist File %s'% newfilename
        os.remove(newfilename)
    rotate_number += 1
    os.rename(filepath,newfilename);

def hexdump(label, raw):
    #uncomment return below to trace read/write
    return
    print '[' + label + ']',
    print ":".join("{0:x}".format(ord(c)) for c in raw )

def push_filter(req):
    buf = sqf.read()

    for cmd in buf[VERSION_LEN:].split( ASYNC_HDLC_FLAG )[:-1]:
        cmd = cmd + ASYNC_HDLC_FLAG
        req.sendall( cmd )
        hexdump("OUT", cmd)
        response = req.recv(1024)
        hexdump("IN", response)

def pull_log(req, f):
    length = 0
    while True:
        try:
            response = req.recv(1024)
            hexdump("IN", response)
            outfile.write(response)
            write_len = len(response)
            if write_len < 0:
                print('-1')
            else:
                #print('write_len',write_len)
                #print('length',length)
                length += write_len
            if length > max_filesize:
                RotateFile(outfile)
                length = 0
        except KeyboardInterrupt:
            print('ctrl-c received, exiting...')
            RotateLastFile(outfile)
            break

#clone from python source, default to socket.AF_INET6
class TCPServer(SocketServer.BaseServer):

    """Base class for various socket-based server classes.

    Defaults to synchronous IP stream (i.e., TCP).

    Methods for the caller:

    - __init__(server_address, RequestHandlerClass, bind_and_activate=True)
    - serve_forever(poll_interval=0.5)
    - shutdown()
    - handle_request()  # if you don't use serve_forever()
    - fileno() -> int   # for select()

    Methods that may be overridden:

    - server_bind()
    - server_activate()
    - get_request() -> request, client_address
    - handle_timeout()
    - verify_request(request, client_address)
    - process_request(request, client_address)
    - shutdown_request(request)
    - close_request(request)
    - handle_error()

    Methods for derived classes:

    - finish_request(request, client_address)

    Class variables that may be overridden by derived classes or
    instances:

    - timeout
    - address_family
    - socket_type
    - request_queue_size (only for stream sockets)
    - allow_reuse_address

    Instance variables:

    - server_address
    - RequestHandlerClass
    - socket

    """
    address_family = socket.AF_INET6

    socket_type = socket.SOCK_STREAM

    request_queue_size = 5

    allow_reuse_address = False

    def __init__(self, server_address, RequestHandlerClass,ipversion, bind_and_activate=True):
        """Constructor.  May be extended, do not override."""
        SocketServer.BaseServer.__init__(self, server_address, RequestHandlerClass)
        if ipversion == 4:
            self.address_family = socket.AF_INET
            print 'IPv4'
        else:
            self.address_family = socket.AF_INET6
            print 'IPv6'
        self.socket = socket.socket(self.address_family,
                                    self.socket_type)
        if bind_and_activate:
            self.server_bind()
            self.server_activate()

    def server_bind(self):
        """Called by constructor to bind the socket.

        May be overridden.

        """
        if self.allow_reuse_address:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind(self.server_address)
        self.server_address = self.socket.getsockname()

    def server_activate(self):
        """Called by constructor to activate the server.

        May be overridden.

        """
        self.socket.listen(self.request_queue_size)

    def server_close(self):
        """Called to clean-up the server.

        May be overridden.

        """
        self.socket.close()

    def fileno(self):
        """Return socket file number.

        Interface required by select().

        """
        return self.socket.fileno()

    def get_request(self):
        """Get the request and client address from the socket.

        May be overridden.

        """
        return self.socket.accept()

    def shutdown_request(self, request):
        """Called to shutdown and close an individual request."""
        try:
            #explicitly shutdown.  socket.close() merely releases
            #the socket and waits for GC to perform the actual close.
            request.shutdown(socket.SHUT_WR)
        except socket.error:
            pass #some platforms may raise ENOTCONN here
        self.close_request(request)

    def close_request(self, request):
        """Called to clean up an individual request."""
        request.close()

class MyTCPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        print('sending filter to modem...')
        push_filter(self.request)
        print('pulling log from modem..., ctrl-c to terminate')
        pull_log(self.request, outfile)

if __name__ == "__main__":
    if len(sys.argv) == 4:
        host, port, sqf, outfile = "", int(sys.argv[1]),\
                open(sys.argv[2], 'rb'), open(sys.argv[3], 'wb')
    elif len(sys.argv) == 5:
        host, port, sqf, outfile, ip_version = "", int(sys.argv[1]),\
                open(sys.argv[2], 'rb'), open(sys.argv[3], 'wb') ,int(sys.argv[4])
    else:
        print 'usage:   python dm-logger.py <port> <filter> <output file>'
        print 'example: python dm-logger.py 2500 MC7xxx_LTE_QOS_filter.sqf out.dm'
        print 'usage:   python dm-logger.py <port> <filter> <output file> <IPv4>'
        print 'example: python dm-logger.py 2500 MC7xxx_LTE_QOS_filter.sqf out.dm 4'
        print 'usage:   python dm-logger.py <port> <filter> <output file> <IPv6>'
        print 'example: python dm-logger.py 2500 MC7xxx_LTE_QOS_filter.sqf out.dm 6'
        sys.exit(-1)
    filepath = sys.argv[3]
    if ip_version == 4:
        server = TCPServer((host, port), MyTCPHandler,4 )
    else:
        server = TCPServer((host, port), MyTCPHandler,6 )
    print('waiting for connection...')
    server.handle_request()
