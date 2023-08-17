# Webserv

### Steps we took
#### Server
- Created a simple C++ server recreation of the minitalk C project, but noticed client connections from nc were being handled LIFO ![nc-connection](https://media.discordapp.net/attachments/1058583385703976980/1141582424732610620/IMG_4460.jpg?width=1596&height=1198)
- Decided to use the select() function to handle multiple connections at once ![nc-connection-concurrent](https://media.discordapp.net/attachments/1058583385703976980/1141627847144902666/IMG_4467.jpg?width=1596&height=1198)