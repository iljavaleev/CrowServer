--  INSERT INTO contact VALUES (DEFAULT, 'name', 'email', 'url');
-- INSERT INTO server_user VALUES (DEFAULT, 'username12', 'email12', 'token12');
-- select * from contact;
-- select * from server_user;
-- select * from user_contact;
-- WITH T as (INSERT INTO contact VALUES (DEFAULT, 'name4', 'email4', 'url4') RETURNING id),
--      C as (SELECT 1 as val) 
-- INSERT INTO user_contact (server_user, contact) SELECT T.id, C.val FROM T CROSS JOIN C;

-- WITH T as (INSERT INTO contact VALUES (DEFAULT, 'name123', 'email123', 'url123') RETURNING id),
--      C as (SELECT 1 as val) 
-- INSERT INTO user_contact (server_user, contact) SELECT  C.val, T.id FROM T CROSS JOIN C;

-- WITH C as (SELECT contact as id FROM user_contact WHERE server_user=1) 
-- SELECT contact.name, contact.email, contact.avatarURL FROM contact 
-- JOIN C on C.id = contact.id;
 

-- WITH uc as (SELECT server_user as id FROM user_contact WHERE contact=10) 
-- SELECT server_user.username, server_user.email, server_user.token FROM server_user
-- JOIN uc on uc.id = server_user.id; 



-- INSERT INTO user_contact (server_user, contact) SELECT  C.val, T.id FROM T CROSS JOIN C;

INSERT INTO contact VALUES (DEFAULT, 'Ryan Florence', 'ryan@reacttraining.com', '../public/ryan.jpg');
INSERT INTO contact VALUES (DEFAULT, 'Michael Jackson', 'michael@reacttraining.com', '../public/michael.jpg');
INSERT INTO contact VALUES (DEFAULT, 'Tyler McGinnis', 'tyler@reacttraining.com', '../public/tyler.jpg');


