# ReadsProfiler

It is a **client-server application** written in **C/C++** that provides access to an online library.

The system supports **advanced book search**, **file download**, and **personalized recommendations** based on user behavior.

The project was developed using **TCP networking (POSIX)** and a **SQLite database**, following a modular and extensible architecture.

## Features
- **User Authentication**
    - login is required before accessing any functionality

- **Advanced Book Search**
    - search by title, author, genre, subgenre, year or rating

- **Book Download**
    - download books using title or ISBN
    - reliable file transfer over TCP

- **Personalized recommendations**
    - generated based on:
        - user search history
        - downloaded books
        - preferred genres
        - frequently accessed authors

## Architecture Overview

ReadsProfiler follows a **client–server architecture** using the **TCP protocol**.

The application is structured on **three logical layers**:
1. **Presentation Layer** - user interaction through client
2. **Logic Layer** - request processing, searching, downloading and recommendations
3. **Persistence Layer** - data storage using SQLite

The **server** handles all computational logic and database operations, while the **client** acts as an interface between the user and the system.

## Project Structure
```text
ReadsProfiler/
|-- client/
|   |-- client.cpp    
|   |-- CommandDispatcher.hpp            
|   |-- CommandDispatcher.cpp             ## Sends requests to the server and manages the TCP connection using send() and recv()
|   |-- ResponseHandler.hpp
|   |__ ResponseHandler.cpp               
|
|-- server/
|   |-- books/   ## The books inserted into the database
    |__ *.txt    ## It contains a short description on every book inserted
|   |-- server.c                       
|   |-- ConnectionManager.hpp
|   |-- ConnectionManager.cpp           ## Manages the communication from TCP
|   |-- DownloadManager.hpp
|   |-- DownloadManager.cpp             ## Downloads the books and send them to the client
|   |-- Protocol.hpp
|   |-- Protocol.cpp                    
|   |-- RecommendationEngine.hpp
|   |-- RecommendationEngine.cpp       ## Generates personalized recommendations for every user, based on their behaviour
|   |-- RequestParser.hpp
|   |-- RequestParser.cpp              ## Validates the request structure
|   |-- SQLiteStorageLayer.hpp
|   |-- SQLiteStorageLayer.cpp         ## Manages the database
|   |-- SearchEngine.hpp
|   |-- SearchEngine.cpp               ## Searches the books based on the criteria
|   |-- library.db
|   |__ seed.sql
|__ README.md

