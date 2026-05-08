# Idea Center Downton Inventory
A simple program to track classes and inventory.
There is one folder for the backend that stores the stembassadors, the classes and the kits
There is another folder for the frontend that requests the info, manages things using a GUI, and let's staff sign in
## Goals
 - [ ] Be able to track the schedules and members of classes
 - [ ] Be able to track where kits are used, how many is left
 - [ ] Track the stembassadors who work at the downtown location of the Idea Center
 - [ ] Keep logs of what happens to prevent fraud
## Problems
 - Need help figuring a system for out who can modify what
## Backend Endpoints:
### /modify-class-members/
allows the system to add or remove people to amount of people in a class
#### body JSON:
``` json
{ 
    "stembassador": {stembassador session ID (string)},
    "count": {amount of people (int)},
    "class": {id of the class (int)}
}
```
#### response JSON:
``` json
{
    "status": {"error" | "success"},
    "message": {info about the status (string)}
}
```

### /add-stembassador/
puts a new stembassador in the system
#### body JSON:
``` json
{ 
    "stembassador": {stembassador session ID (string)},
    "name": {new stembassador name (string)},
    "password": {new stembassador password (string)}
}
```
#### response JSON:
``` json
{
    "status": {"error" | "success"},
    "message": {info about the status (string)}
}
```

### /login/
gives a session id to allow the frontend to be logged in
#### body JSON:
``` json
{ 
    "stembassador": {stembassador session ID (string)},
    "password": {stembassador password (string)}
}
```
#### response JSON:
``` json
{
    "status": {"error" | "success"},
    "message": {info about the status (string)},
    "session-id": {the ID of the logged in session (string)}
}
```

