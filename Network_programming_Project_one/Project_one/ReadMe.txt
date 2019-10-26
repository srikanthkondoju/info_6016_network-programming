after downloading this project from git
1) open Project_one.sln
2) there are three projects
  a) Buffer - it is a library for serilizing      and deserilizing messages.
  b) Client - it is for the client.
  c) Server - it is for server.

build all three projects.

then goto x64/Debug
there you will find two executable files
ServerExample.exe
ClientExample.exe

first run ServerExample.exe
now server is ready and waiting for clients.
now run client program(as many instances as you want)
as soon as they are connected to the sever
the program will ask you a name for client
give client a name
then you are given with three option
1)join a room
2)leave a room
3)send message to the room

you can choose any of the option
before sending or leaving the room you need to join the room. if there is any update regarding the room then server will broadcast it to all the client in that room.