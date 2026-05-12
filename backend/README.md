# Idea Center Management Backend
A simple program to track classes, sign in members, and track inventory.

## Goals
 - [ ] Be able to track the schedules and members of classes
 - [ ] Be able to track where kits are used, how many is left
 - [x] Track the stembassadors who work at the location of the Idea Center
 - [ ] Keep logs of what happens to prevent fraud
 - [ ] Manage members of the idea center
 - [ ] Sign in staff and and members
 
## Problems
 - Need help figuring a system for out who can modify what

## Endpoints
### POST /modify-class-members/
allows the system to add or remove people to amount of people in a class
#### body JSON:
``` json
{ 
    "stembassador": {stembassador name (string)},
    "session-id": {stembassador session ID (string)},
    "count": {modification of people (int)},
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

### /login-stembassador/
gives a session id to allow the frontend to be logged in
#### body JSON:
``` json
{ 
    "name": {stembassador name (string)},
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

### /create-kit/
creates a new kit
#### body JSON:
``` json
{ 
    "stembassador": {stembassador name (int)},
    "session-id": {stembassador session ID (string)},
    "kit-info": {
        "name": {kit name (string)},
        "length": {time to make kit (int, if 0 then there is no set time)},
        "price": {cost for 1 kit (int, if 0 then the kit is not for sale)},
        "stock": {amounf of kits (int)}
    }
}
```
#### response JSON:
``` json
{
    "status": {"error" | "success"},
    "message": {info about the status (string)}
}
```


### /remove-stembassador/
removes an exisiting stembassador
#### body JSON:
``` json
{ 
    "stembassador": {stembassador name (string)},
    "session-id": {stembassador session ID (string)},
    "to-remove": {stembassador name to remove (string))}
}
```
#### response JSON:
``` json
{
    "status": {"error" | "success"},
    "message": {info about the status (string)}
}
```