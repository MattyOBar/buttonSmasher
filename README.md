#ButtonSmasher

ButtonSmasher is based around an Arduino Mega 2560, using an Arduino Ethernet Shield to for LAN connectivity.  There is a Flask application that the user can run inside of a Docker container that acts as a frontend and a backend to display instanced results from the user's session.  There is also an instance of Redis running inside of a docker container, that is being used as a database to store data from the user's session.
