-- psql -U postgres -tc "SELECT 1 FROM pg_database WHERE datname = 'react'" | grep -q 1 || psql -U postgres -c "CREATE DATABASE react"
-- psql -U postgres -d react -a -f db_setup.sql    
DROP TABLE IF EXISTS server_user cascade;
DROP TABLE IF EXISTS contact cascade;
DROP TABLE IF EXISTS user_contact;

CREATE TABLE server_user(
    id SERIAL PRIMARY KEY,
    username varchar(128),
    email varchar(255) UNIQUE,
    token text
);

CREATE TABLE contact(
    id SERIAL PRIMARY KEY,
    name varchar(32),
    email varchar(255) UNIQUE,
    avatarURL varchar(255)
);

CREATE TABLE user_contact(
    server_user bigint,
    contact bigint,
    FOREIGN KEY (server_user) REFERENCES server_user (id) ON DELETE CASCADE,
    FOREIGN KEY (contact) REFERENCES contact (id) ON DELETE CASCADE,
    PRIMARY KEY (server_user, contact)
);
