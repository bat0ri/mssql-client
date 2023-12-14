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

## PART ONE

- **Tables Creation**: Utilized SQL queries to create three tables incorporating concepts like NOT NULL, DEFAULT, PRIMARY KEY, CHECK, and IDENTITY.
- **Inter-table Relations**: Established two inter-table relationships:
  1. One without systemic cascading deletion and updating.
  2. Another with systemic cascading deletion and updating (ON DELETE CASCADE ON UPDATE CASCADE).
- **Stored Procedure Creation**: Developed a stored procedure with output parameters.

## PART TWO

- **Client-Side Implementation**:
  - Developed a user interface with a main screen form displaying tables from the database.
  - Implemented database connection using QODBC driver to QSqlDatabase datatype and employed QSqlTableModel for each table with an OnManualSubmit hashing strategy.

- **Alternative Form and Views**:
  - Created an alternative form dynamically displaying all tables in one QSqlTableModel.
  - Established a VIEW on the server, presenting data from three tables as a single connected table and displayed it on a separate form.
  - Displayed the output of a stored procedure in a Label on the main or alternative form.

### Task Implementations

1. **Task 1**:
   - Utilized SQL queries to gather information about deliveries from contracts ending in the current year.

2. **Task 2**:
   - Employed SQL queries with grouping or window functions to collect information about deliveries from contracts ending in the current year.

3. **Task 3**:
   - Implemented various methods to retrieve information about the availability of parts at warehouses based on specific criteria, including expression RA, RIK language query, and SQL queries with subqueries.
