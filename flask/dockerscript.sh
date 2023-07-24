#Run this script with docker desktop running, to start the redis and flask containers, and then show the docker logs for the flask container.
docker compose down
docker compose up --build -d
docker logs flask -f