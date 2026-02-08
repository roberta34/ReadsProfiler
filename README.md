#ReadsProfiler

It is a **client-server application** written in **C/C++** that provides access to an online library.

The system supports **advanced book search**, **file download**, and **personalized recommendations** based on user behavior.

The project was developed using **TCP networking (POSIX)** and a **SQLite database**, following a modular and extensible architecture.

#Features
- **User Authentification**
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
    - preffered genres
    - frequently accessed authors

#Architecture Overview

ReadsProfiler follows a **client–server architecture** using the **TCP protocol**.
