--  INSERT INTO contact VALUES (DEFAULT, 'name', 'email', 'url');
-- INSERT INTO server_user VALUES (DEFAULT, 'username12', 'email12', 'token12');
-- select * from contact;
-- select * from server_user;
-- select * from user_contact;
WITH T as (INSERT INTO contact VALUES (DEFAULT, 'name4', 'email4', 'url4') RETURNING id),
     C as (SELECT 1 as val) 
-- INSERT INTO user_contact (server_user, contact) SELECT T.id, C.val FROM T CROSS JOIN C;

-- WITH T as (INSERT INTO contact VALUES (DEFAULT, 'name123', 'email123', 'url123') RETURNING id),
--      C as (SELECT 1 as val) 
-- INSERT INTO user_contact (server_user, contact) SELECT  C.val, T.id FROM T CROSS JOIN C;

WITH C as (SELECT contact as id FROM user_contact WHERE server_user=1) 
SELECT contact.name, contact.email, contact.avatarURL FROM contact 
JOIN C on C.id = contact.id;
 

WITH uc as (SELECT server_user as id FROM user_contact WHERE contact=10) 
SELECT server_user.username, server_user.email, server_user.token FROM server_user
JOIN uc on uc.id = server_user.id; 
--  {
--       id: 'ryan',
--       name: 'Ryan Florence',
--       email: 'ryan@reacttraining.com',
--       avatarURL: config.origin + '/ryan.jpg'
--     },
--     {
--       id: 'michael',
--       name: 'Michael Jackson',
--       email: 'michael@reacttraining.com',
--       avatarURL: config.origin + '/michael.jpg'
--     },
--     {
--       id: 'tyler',
--       name: 'Tyler McGinnis',
--       email: 'tyler@reacttraining.com',
--       avatarURL: config.origin + '/tyler.jpg'
--     }

