#
# $Id$
#
# Prints svn tag name and svn revision  
#   E.g "v0.0 r621"
#

#buildtag_none = "(no tag)"
buildtag_none = "1.0"

#
# Get Tag string from workspace filesystem
#
# Inputs:
#  - none
#
# Output:
#  - Build Tag or 0.0 if the version is untagged
#
def GetTagString(path=None):
    import re, os

    # assume no tag, caller to substitute string
    buildtag = buildtag_none

    rev = None
    if path is None:
        path = '.'
    entries_path = '%s/mkver.txt' % path

    if os.path.exists(entries_path):
        entries = open(entries_path, 'r').read()
        rev_match = re.search('URL: .*/tags/(.*)', entries)
        if rev_match:
            buildtag = rev_match.groups()[0]

    print 'v'+buildtag

#
# Get Repository revision from workspace filesystem
#
# Inputs:
#  - none
#
# Output:
#  - Repository revision or 'unknown'
#
def get_svn_revision(path=None):
    import re, os
    rev = None
    if path is None:
        path = '.'
    entries_path = '%s/mkver.txt' % path

    if os.path.exists(entries_path):
        entries = open(entries_path, 'r').read()
        rev_match = re.search('Last Changed Rev: (\d+)', entries)
        if rev_match:
            rev = rev_match.groups()[0]

    if rev:
        print u'r%s' % rev
        return
    print u'r0'

###################
# Main code
###################

# Retrieve Build tag
buildtag = GetTagString()

# Retrieve repository revision
revision = get_svn_revision()

