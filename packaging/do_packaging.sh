#!/bin/sh

DIR="$(cd "$(dirname "$0")" && pwd)"

tar -cO $DIR/../ | docker run --rm=true -i centos:6 /bin/bash -c '
yum install -y tar cmake lua-devel pcp-libs-devel gcc rpm-build 1>&2 &&
mkdir pcp-machine &&
cd pcp-machine/ &&
cat /proc/self/fd/0 | tar -xf - &&
find . -iwholename "*cmake*" -not -name CMakeLists.txt -delete 1>&2 &&
rpmbuild -bb pcp-machine.spec 1>&2 &&
cat /root/rpmbuild/RPMS/x86_64/pcp-machine-0.1-1.x86_64.rpm
' > $DIR/pcp-machine-0.1-1.x86_64.rpm