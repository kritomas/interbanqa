begin transaction;

create table Account
(
	id integer primary key check(id between 9999 and 99999),
	balance bigint not null default 0 check(balance >= 0),

	is_dummy bit not null default 0
);

insert into Account (id, is_dummy) values (9999, 1); -- Dummy insert

create view Balance_Total as
	select ifnull(sum(balance), 0) from Account where is_dummy = 0;

create view Account_Total as
	select count(*) from Account where is_dummy = 0;

commit;