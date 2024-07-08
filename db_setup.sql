-- psql -U postgres -tc "SELECT 1 FROM pg_database WHERE datname = 'react'" | grep -q 1 || psql -U postgres -c "CREATE DATABASE react"
-- psql -U postgres -d react -a -f db_setup.sql    
DROP TABLE IF EXISTS server_user cascade;
DROP TABLE IF EXISTS contact cascade;
DROP TABLE IF EXISTS user_contact;
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE server_user(
    email varchar(128) UNIQUE,
    passw varchar(128) UNIQUE,
    salt uuid
);

CREATE TABLE contact(
    id SERIAL PRIMARY KEY,
    name varchar(32),
    email varchar(128) UNIQUE,
    avatarURL varchar(255)
);

CREATE TABLE user_contact(
    user_email varchar(128),
    contact bigint,
    FOREIGN KEY (user_email) REFERENCES server_user (email) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (contact) REFERENCES contact (id) ON DELETE CASCADE,
    PRIMARY KEY (user_email, contact)
);
