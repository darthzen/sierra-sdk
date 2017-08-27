#*******************************************************************************
# Copyright (c) 2012 Sierra Wireless and others.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************

PROJECT(CFWK_DEVICETREE)

ADD_LIBRARY(Swi_Util SHARED swi_util.c)
ADD_PUBLIC_HEADER(Swi_Util swi_util.h)
TARGET_LINK_LIBRARIES(Swi_Util Emp lib_yajl lib_swi_log)

ADD_UNIT_TEST(dt_util dt_util.c RUNTIME_DEPENDENCIES Swi_Util lib_swi_log)
