set -e
docker build --build-arg USER_ID=`id -u` -t welder .
