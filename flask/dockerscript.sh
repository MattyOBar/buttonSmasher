docker compose down
docker build --tag whackamole . 
docker compose up -d
docker logs flask -f