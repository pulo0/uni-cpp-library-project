CREATE TABLE IF NOT EXISTS Books(
    id INTEGER PRIMARY KEY,
    title TEXT,
    author TEXT,
    description TEXT,
    year_of_publish INTEGER,
    cover TEXT,
    is_available NUMERIC NOT NULL
);

CREATE TABLE IF NOT EXISTS Users(
    id INTEGER PRIMARY KEY,
    login TEXT NOT NULL UNIQUE,
    password TEXT,
    is_admin NUMERIC NOT NULL
);

CREATE TABLE IF NOT EXISTS Rental(
    id INTEGER PRIMARY KEY,
    id_book INTEGER UNIQUE,
    id_user INTEGER,
    return_date TEXT NOT NULL,
    is_prolonged NUMERIC NOT NULL,

    FOREIGN KEY (id_book) REFERENCES Book(id),
    FOREIGN KEY (id_user) REFERENCES User(id)
);