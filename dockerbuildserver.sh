set -e
docker load -i ./Build/Cache/zero.tar || true
./dockerbuild.sh
docker save -o ./Build/Cache/zero.tar zero
