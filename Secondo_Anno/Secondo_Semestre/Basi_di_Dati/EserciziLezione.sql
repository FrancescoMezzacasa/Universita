SHOW DATABASES;
USE classicmodels;
select * from employees where firstName = 'Leslie';
select * from employees where lastName like '%son';
select * from employees where firstName like '_arry';
select * from products where productScale like '1:_0';
select * from employees where firstName like 'M_r%';
select productName, MSRP from products where MSRP < 75 or MSRP > 150;

select productName, MSRP, buyPrice
from products
where (MSRP < 75 or MSRP > 150) and buyPrice > 30;

select * from payments where amount between 5000 and 8000;
select * from employees where firstName between 'b' and 'f';
select officeCode, city, phone from offices where country in ('USA', 'France');

SELECT *
FROM orders
WHERE shippedDate IS NULL;

select *
from orders
where orderDate > '2005/04/30' and shippedDate is NULL;

select productName, MSRP, buyPrice
from products
where (MSRP - buyPrice) > 50;

select productName, length(productName) as nameLen from products where length(productName) >= 15;

select * from products, orders where monthname(orderDate) = 'January';

select productName, MSRP
from products
order by MSRP;

select customerName, country, creditLimit
from customers
order by country ASC, creditLimit DESC;

select distinct status
from orders;

select *
from orders
order by FIELD(status, 'In Process', 'On Hold', 'Cancelled', 'Resolved', 'Disputed', 'Shipped');

select distinct city
from customers
order by city;

select * from customers, employees;

select c.customerName, e.employeeNumber, e.lastName
from customers c inner join employees e
where c.salesRepEmployeeNumber = e.employeeNumber;

select p.productCode, p.productName, l.textDescription
from products p
inner join productlines l
on p.productLine = l.productLine;

select e.firstName, e.lastName, o.city
from employees e inner join offices o
using(officeCode);

select customerName, concat(firstName, ' ', lastName) as employeeFullName
from customers
left outer join employees
on salesRepEmployeeNumber = employeeNumber;

SELECT c.customerNumber, c.customerName, o.orderNumber, o.status
FROM customers c
LEFT OUTER JOIN orders o
ON c.customerNumber = o.customerNumber
WHERE o.orderNumber IS NULL;

SELECT c.customerName, CONCAT(e.firstName, ' ', e.lastName) as fullName, o.phone
FROM customers c
LEFT OUTER JOIN employees e
ON c.salesRepEmployeeNumber = e.employeeNumber
LEFT OUTER JOIN offices o
USING(officeCode);

SELECT t1.employeeNumber, concat(t1.firstName, ' ', t1.lastName) as employeeName, t2.employeeNumber, concat(t2.firstName, ' ', t2.lastName) as bossName
FROM employees t1
LEFT JOIN employees t2
ON t1.reportsTo = t2.employeeNumber;

SELECT t1.customerName, t2.customerName, t1.city
FROM customers t1
INNER JOIN customers t2
using(city)
WHERE t1.customerName != t2.customerName;

SELECT employeeNumber as id, firstName as name
FROM employees
UNION
SELECT customerNumber as id, contactLastName as name
FROM customers;

SELECT country
FROM offices
UNION
SELECT country
FROM customers
ORDER BY country;

SELECT orders.status
FROM orders
GROUP BY status;

SELECT status
FROM orders
WHERE orderDate < '2003-12-31'
GROUP BY status;

SELECT COUNT(*) as Count
FROM employees;

SELECT COUNT( distinct checkNumber)
FROM payments;

SELECT SUM(amount)
FROM payments;

SELECT AVG(MSRP)
FROM products;

SELECT AVG(MSRP), MAX(MSRP), MIN(MSRP)
FROM products;

SELECT status, count(status)
FROM orders
GROUP BY status;

SELECT productLine, COUNT(*), AVG(MSRP)
FROM products
GROUP BY productLine;

SELECT COUNT(*), shippedDate
FROM orders
GROUP BY shippedDate;

SELECT COUNT(*), month(shippedDate)
FROM orders
WHERE month(shippedDate) IS NOT NULL
GROUP BY month(shippedDate);

SELECT month(shippedDate), year(shippedDate), COUNT(*)
FROM orders
WHERE shippedDate IS NOT NULL
GROUP BY month(shippedDate), year(shippedDate);

SELECT orderNumber, SUM(quantityOrdered * priceEach)
FROM orderdetails
GROUP BY orderNumber;

SELECT customerName, orderDate, SUM(quantityOrdered * priceEach)
FROM customers
INNER JOIN orders
USING(customerNumber)
INNER JOIN orderdetails
USING(orderNumber)
GROUP BY orderNumber;

SELECT customerNumber, COUNT(customerNumber)
FROM orders
GROUP BY customerNumber
ORDER BY COUNT(customerNumber) desc;

SELECT p.paymentDate, -1 * p.amount
FROM payments p
WHERE p.customerNumber = 124;

SELECT o.orderDate, o.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale
FROM orders o
INNER JOIN orderdetails od
using(orderNumber)
WHERE o.customerNumber = 124
GROUP BY o.orderNumber;

SELECT p.paymentDate AS data, -1 * p.amount AS somma
FROM payments p
WHERE p.customerNumber = 124
UNION
SELECT o.orderDate as data, sum(od.priceEach * od.quantityOrdered) as somma
FROM orders o
INNER JOIN orderdetails od
using(orderNumber)
WHERE o.customerNumber = 124
GROUP BY o.orderNumber
ORDER BY data;

SELECT od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale
FROM orderdetails od
GROUP BY od.orderNumber
HAVING totale < 10000;

SELECT od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale, sum(od.quantityOrdered)
FROM orderdetails od
GROUP BY od.orderNumber
HAVING totale < 10000 and sum(od.quantityOrdered) > 100;

<SELECT od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale, o.status
FROM orderdetails od
INNER JOIN orders o
USING(orderNumber)
GROUP BY od.orderNumber
HAVING totale < 10000 and o.status <> 'shipped';

SELECT p.productName, p.MSRP, (SELECT max(MSRP) FROM products) as massimo
FROM products p;

SELECT p.customerNumber, p.checkNumber, p.amount
FROM payments p
WHERE p.amount = (SELECT max(amount) FROM payments);

SELECT customerNumber, checkNumber, amount
FROM payments
WHERE amount > (SELECT AVG(amount) FROM payments);

SELECT c.customerName
FROM customers c
WHERE c.customerNumber NOT IN (SELECT DISTINCT customerNumber FROM orders);

SELECT max(quantita), min(quantita), avg(quantita)
FROM (SELECT sum(quantityOrdered) as quantita FROM orderdetails GROUP BY orderNumber) as qo;

SELECT p.productName, p.buyPrice
FROM products p
WHERE p.buyPrice > (SELECT avg(p1.buyPrice)
                    FROM products p1
                    WHERE p1.productLine = p.productLine
                    GROUP BY p1.productLine);

SELECT customerNumber, customerName, creditLimit
FROM customers
ORDER BY creditLimit DESC
LIMIT 5;

SELECT productName, buyPrice
FROM products
ORDER BY buyPrice DESC
LIMIT 1, 1;

/*INSERT INTO offices
VALUES (8, 'Trieste', '+39 043155555', 'Via della stazione 5',
        null, null, 'Italy', '33333', 'EU');*/

SELECT * FROM orderdetails WHERE orderNumber = 10426;

INSERT INTO orders
VALUES (10426, '2005-05-31', '2005-06-07',
        null, 'In Process', null, 119);

SELECT 10426 as orderNumber, productCode, quantityOrdered, priceEach, orderLineNumber
FROM orderdetails
WHERE orderNumber = 10425;

INSERT INTO orderdetails
    SELECT 10426 as orderNumber, productCode, quantityOrdered, priceEach, orderLineNumber
    FROM orderdetails
    WHERE orderNumber = 10425;

UPDATE employees
SET email = 'marypatterson@classicmodelcars.com'
WHERE firstName = 'Mary' and lastName = 'Patterson';

SELECT email
FROM employees
WHERE firstName = 'Mary' and lastName = 'Patterson';

update products
set buyPrice = 100, MSRP = 230
where productName = '2001 Ferrari Enzo';

UPDATE products
SET MSRP = MSRP*1.05;

SELECT *
FROM customers
WHERE salesRepEmployeeNumber IS NULL;

select employees.jobTitle
from employees;

SELECT employeeNumber
FROM employees
WHERE jobTitle = 'Sales Rep'
ORDER BY employeeNumber DESC
LIMIT 1;

UPDATE customers
SET salesRepEmployeeNumber = (SELECT employeeNumber
    FROM employees
    WHERE jobTitle = 'Sales Rep'
    ORDER BY employeeNumber DESC
    LIMIT 1)
WHERE salesRepEmployeeNumber IS NULL;

SELECT *
FROM customers
WHERE country = 'Italy';

DELETE FROM customers
WHERE country = 'Italy'; /*Non si può*/

SELECT e.firstName, e.lastName, od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale
FROM employees e
INNER JOIN customers c on e.employeeNumber = c.salesRepEmployeeNumber
INNER JOIN orders o on c.customerNumber = o.customerNumber
INNER JOIN orderdetails od on o.orderNumber = od.orderNumber
GROUP BY od.orderNumber
ORDER BY totale DESC
LIMIT 1;

(SELECT e.firstName, e.lastName, od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale
FROM employees e
INNER JOIN customers c on e.employeeNumber = c.salesRepEmployeeNumber
INNER JOIN orders o on c.customerNumber = o.customerNumber
INNER JOIN orderdetails od on o.orderNumber = od.orderNumber
GROUP BY od.orderNumber
ORDER BY totale DESC
LIMIT 1)
UNION
(SELECT e.firstName, e.lastName, od.orderNumber, sum(od.priceEach * od.quantityOrdered) as totale
FROM employees e
INNER JOIN customers c on e.employeeNumber = c.salesRepEmployeeNumber
INNER JOIN orders o on c.customerNumber = o.customerNumber
INNER JOIN orderdetails od on o.orderNumber = od.orderNumber
GROUP BY od.orderNumber
ORDER BY totale
LIMIT 1);

SELECT e.firstName, e.lastName, c.customerName, o.orderNumber,
       sum(od.priceEach * od.quantityOrdered) as totale
FROM employees e
INNER JOIN customers c on e.employeeNumber = c.salesRepEmployeeNumber
INNER JOIN orders o on c.customerNumber = o.customerNumber
INNER JOIN orderdetails od on o.orderNumber = od.orderNumber
GROUP BY o.orderNumber
HAVING totale > (SELECT avg(total)
                 FROM (SELECT sum(priceEach * quantityOrdered) as total
                       FROM orderdetails GROUP BY orderNumber) as TabellaTotali)
ORDER BY totale DESC;


SELECT o.orderNumber, od.productCode, p.productName, od.quantityOrdered, od.priceEach,
       (od.priceEach * od.quantityOrdered) as totale
FROM orders o
INNER JOIN orderdetails od USING(orderNumber)
INNER JOIN products p USING(productCode)
WHERE od.priceEach * od.quantityOrdered = (SELECT max(priceEach * quantityOrdered)
                FROM orderdetails
                WHERE orderNumber = o.orderNumber)
ORDER BY orderNumber;

/*soluzione fatta dal prof.
esercizio:
MOSTRA NUMERO DELL'UFFICIO, TOTALE ORDINE E NOME E COGNOME DEL VENDITORE
CHE HA CHIUSO L'ORDINE PIÙ GRANDE PER OGNI UFFICIO.*/
SELECT o.officeCode,e.firstName, e.lastName,t.orderValue
FROM offices o
JOIN employees e using (officeCode)
JOIN customers c ON c.salesRepEmployeeNumber = e.employeeNumber
JOIN (
    SELECT
        o2.customerNumber,
        SUM(od.quantityOrdered * od.priceEach) AS orderValue,
        o2.orderNumber
    FROM orders o2
    JOIN orderdetails od USING (orderNumber)
    GROUP BY o2.orderNumber
) t
    USING (customerNumber)
WHERE t.orderValue = (
    SELECT MAX(t2.orderValue)
    FROM (
        SELECT
            SUM(quantityOrdered * priceEach) AS orderValue,
            officeCode
        FROM orders JOIN orderdetails USING(orderNumber)
        JOIN customers USING (customerNumber)
        JOIN employees
            ON employeeNumber = salesRepEmployeeNumber
        GROUP BY orderNumber, officeCode /*devo raggruppare anche per office code
                                           se no non potrei selezionarlo*/
    ) t2
    WHERE t2.officeCode = o.officeCode
);

PREPARE STMT1 FROM
    'SELECT productCode, productName FROM products
    WHERE MSRP > ?';

SET @prezzo = 100;

EXECUTE STMT1 USING @prezzo;

CREATE VIEW viewTotaleOrdini AS
    SELECT orderNumber, sum(priceEach*quantityOrdered) as totale
    FROM orders o
    INNER JOIN orderdetails od USING(orderNumber)
    GROUP BY orderNumber;
SELECT * FROM viewTotaleOrdini;
SELECT * FROM viewTotaleOrdini
WHERE orderNumber = 10102;

CREATE USER pippo@localhost
IDENTIFIED BY 'cicciogamer';

GRANT SELECT, UPDATE, DELETE
ON classicmodels.*
TO pippo@localhost;

CREATE PROCEDURE selectSalesReps()
BEGIN
    SELECT * FROM employees
    WHERE jobTitle = 'Sales Rep';
end;

CALL selectSalesReps();

SHOW PROCEDURE STATUS;

CREATE PROCEDURE selectEmployee(ruolo varchar(30))
begin
    SELECT * FROM employees
    WHERE jobTitle = ruolo;
end;

DROP PROCEDURE selectEmployee;
CALL selectEmployee('Sales Manager (APAC)');
SELECT DISTINCT employees.jobTitle FROM employees;

CREATE PROCEDURE contatore(INOUT value int, IN offset int)
BEGIN
    SET value = value + offset;
end;

DROP PROCEDURE contatore;
SET @conto = 0;

CALL contatore(@conto, 10);
SELECT @conto;

CREATE PROCEDURE raddoppia(INOUT value int)
BEGIN
    SET value = 2 * value;
end;

SET @valore = 2;
CALL raddoppia(@valore);
SELECT @valore;

CREATE PROCEDURE totalItems(IN numeroOrdine int, OUT itemsNumber int)
BEGIN
    SET itemsNumber = (SELECT sum(quantityOrdered)
    FROM orderdetails od
    WHERE od.orderNumber = numeroOrdine
    GROUP BY od.orderNumber);
end;

DROP Procedure totalItems;
SET @quantita = 0;
CALL totalItems(10100, @quantita);
SELECT @quantita;

CREATE PROCEDURE numberByStatus(IN status varchar(50), OUT number int)
BEGIN
    SELECT count(*) INTO number
    FROM orders
    WHERE orders.status = status;
end;

SET @numero = 0;
CALL numberByStatus('Shipped', @numero)
SELECT @numero;

CREATE PROCEDURE customerInfo(IN customerCode int, OUT shippedNumber int,
    OUT canceledNumber int, OUT resolvedNumber int, OUT disputedNumber int)
BEGIN
    SELECT count(*) INTO shippedNumber
    FROM orders
    WHERE orders.status = 'Shipped' and orders.customerNumber = customerCode;

    SELECT count(*) INTO canceledNumber
    FROM orders
    WHERE orders.status = 'Canceled' and orders.customerNumber = customerCode;

    SELECT count(*) INTO resolvedNumber
    FROM orders
    WHERE orders.status = 'Resolved' and orders.customerNumber = customerCode;

    SELECT count(*) INTO disputedNumber
    FROM orders
    WHERE orders.status = 'Disputed' and orders.customerNumber = customerCode;
end;

DROP PROCEDURE customerInfo;
CALL customerInfo(141, @shipped, @canceled, @resolved, @disputed)
SELECT @shipped, @canceled, @resolved, @disputed;

CREATE PROCEDURE getCustomerLevel(IN numeroCliente int, OUT livello varchar(10))
BEGIN
    DECLARE credito float;
    SET credito = (SELECT c.creditLimit FROM customers c WHERE c.customerNumber = numeroCliente);
    IF credito > 50000 THEN
        SET livello = 'PLATINUM';
    ELSEIF credito > 10000 and credito <= 50000 THEN
        SET livello = 'GOLD';
    ELSE
        SET livello = 'SILVER';
    end if;
end;

CALL getCustomerLevel(103, @livello);
SELECT @livello;

CREATE PROCEDURE totalOrders(IN numeroCliente int, OUT numeroOrdini int)
BEGIN
    DECLARE conteggio int;
    SELECT count(*) INTO conteggio
    FROM orders o
    WHERE o.customerNumber = numeroCliente;

    IF conteggio = 0 THEN
        SIGNAL SQLSTATE '45001'
        SET MESSAGE_TEXT = 'Errore nella ricerca';
    END IF;
end;

CALL totalOrders(12, @num);

-- cursore che mette tutte le email in una variabile sola
CREATE PROCEDURE mergeEmails(INOUT emails varchar(1000))
BEGIN
    DECLARE finished INT DEFAULT 0;
    DECLARE singolaEmail VARCHAR(255);
    DECLARE unisciEmail CURSOR FOR
        SELECT email FROM employees;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET finished = 1;
    SET emails = '';
    OPEN unisciEmail;
    WHILE finished = 0 DO
        FETCH unisciEmail INTO singolaEmail;
        IF emails = '' THEN
            SET emails = singolaEmail;
        end if;
        SET emails = CONCAT(emails, ';', singolaEmail);
    END WHILE;
    CLOSE unisciEmail;
end;

DROP procedure mergeEmails;
CALL mergeEmails(@emails);
SELECT @emails;

CREATE FUNCTION raddoppiaUDF(numero float) RETURNS float DETERMINISTIC
BEGIN
    RETURN numero * 2;
end;

DROP FUNCTION raddoppiaUDF;
SELECT raddoppiaUDF(4.14);
SELECT creditLimit from customers;
SELECT raddoppiaUDF(creditLimit) from customers;

CREATE FUNCTION getCustomerLevelUDF(credito float) RETURNS varchar(15) DETERMINISTIC
BEGIN
    IF credito > 50000 THEN
        return 'PLATINUM';
    ELSEIF credito > 10000 and credito <= 50000 THEN
        return 'GOLD';
    ELSE
        return 'SILVER';
    END IF;
end;
SELECT customerNumber, creditLimit, getCustomerLevelUDF(creditLimit) FROM customers;

CREATE FUNCTION numberOfOrdersUDF(numeroCliente int) RETURNS int DETERMINISTIC
BEGIN
    DECLARE conto int;
    SELECT count(*) INTO conto FROM orders
    WHERE customerNumber = numeroCliente;
    RETURN conto;
end;
SELECT customerNumber, numberOfOrdersUDF(customerNumber) FROM customers;

-- trigger che impedisce la modifica del credito
CREATE TRIGGER impedisciModificaCredito
BEFORE UPDATE ON customers
FOR EACH ROW
BEGIN
    IF NEW.creditLimit > OLD.creditLimit THEN
        SIGNAL sqlstate '45001' SET message_text = 'Non puoi modificare il credito!';
    end if;
end;

SELECT sum(quantity)
FROM orderdetails od
JOIN orders o USING(orderNumber)
GROUP BY o.customerNumber;

SELECT max(quantita), min(quantita), floor(avg(quantita))
FROM (SELECT sum(quantityOrdered) AS quantita
FROM orderdetails od
JOIN orders o USING(orderNumber)
GROUP BY o.customerNumber) q;