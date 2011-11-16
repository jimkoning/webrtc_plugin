#!/bin/sh

# Test the existence of certain items and fail if appropriate.
EXITVAL=0

which git >/dev/null
if [ $? != 0 ]
then
  echo
  echo "'git' is required and was not found in the path. (see http://www.git-scm.org)"
  EXITVAL=1
fi

which gclient >/dev/null
if [ $? != 0 ]
then
  echo
  echo "'gclient' is required and was not found in the path."
  echo "  See https://sites.google.com/a/chromium.org/dev/developers/how-tos/install-depot-tools"
  echo "  or 'svn co http://src.chromium.org/svn/trunk/tools/depot_tools' and then"
  echo "    add depot_tools to your path -- 'export PATH=\$PATH:<path-to-depot_tools'"
  EXITVAL=1
fi

which doxygen
if [ $? != 0 ]
then
  echo
  echo WARNING: 'doxygen' does not exist or not in path. Not required but excellent tool for docs.
  echo
fi

exit $EXITVAL
