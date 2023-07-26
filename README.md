ButtonSmasher

![Screen Shot 2023-07-26 at 2 59 55 PM](https://github.com/MattyOBar/buttonSmasher/assets/101294899/86b8b0e7-1e12-4559-a67c-54f89b1b7bdb)

ButtonSmasher is based around an Arduino Mega 2560, using an Arduino Ethernet Shield to for LAN connectivity.  The web app is a Flask application that is being hosted locally inside of a Docker container.  Redis is being used as a database to store user data from each session.

To install and run ButtonSmasher locally, you simply need to clone this repo, open up docker desktop, and run this command:

`bash dockerscript.sh`


Here is an image with the wiring diagram for all the components that make up ButtonSmasher:

![buttonSmasher__bread_board](https://github.com/MattyOBar/buttonSmasher/assets/101294899/29a5c2ed-ce73-4860-b1c9-0e203de73144)
