# Qt Database Client Application

This Qt-based application serves as a versatile client for database management, enabling users to execute SQL queries and utilize Object-Relational Mapping (ORM) functionalities. 

## Features

- **SQL Query Execution**: Execute custom SQL queries directly through the application.
- **ORM Support**: Utilize Object-Relational Mapping to interact with the database programmatically.
- **Multi-Database Compatibility**: Compatible with various database systems, such as SQL Server, MySQL, PostgreSQL, etc.

## Usage

### Installation
1. Clone this repository.
2. Install necessary dependencies. (Provide details if any)
3. Configure the database connection by modifying the provided connection string in the code.


```cpp
db = QSqlDatabase::addDatabase("QODBC");
QString connectString = "DRIVER={SQL Server};";
connectString.append("SERVER=YOUR_SERVER_NAME\YOUR_SQL_INSTANCE;"); // Hostname, SQL-Server Instance
connectString.append("DATABASE=your_database_name;"); // Schema
connectString.append("UID=your_username;"); // User
connectString.append("PWD=your_password;"); // Pass
connectString.append("Trusted_Connection=Yes;");
db.setDatabaseName(connectString);
```

Simply replace YOUR_SERVER_NAME, YOUR_SQL_INSTANCE, your_base_name, your_username and your_password with the appropriate values ​​for your database data.
