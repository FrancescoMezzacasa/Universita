SHOW DATABASES;
USE unidb;
SHOW TABLES ;
DESCRIBE corsi;

/*Esercizio 1*/
SELECT COUNT(*)
FROM studenti
WHERE (matricola LIKE 'IN05%') and
      ((cf LIKE '________A%') or
       (cf LIKE '________B%') or
       (cf LIKE '________C%'));

/*Esercizio 2*/
SELECT s.nome, s.cognome
FROM studenti s
INNER JOIN esami e
ON s.matricola = e.studente
INNER JOIN corsi c
ON e.corso = c.codice
WHERE c.nome = 'Basi di Dati' and e.lode = TRUE;

/*Esercizio 3*/
SELECT p.nome, p.cognome
FROM professori p
LEFT JOIN corsi c
ON p.matricola = c.professore
WHERE c.codice IS NULL;

/*Esercizio 4*/
SELECT SUBSTRING(matricola, 1, 4) as corsoDiStudi, COUNT(*) as conto
FROM studenti
GROUP BY corsoDiStudi
ORDER BY conto DESC;

/*Esercizio 5*/
SELECT CONCAT('Prof. ', p.nome, ' ', p.cognome) as Titolo, max(e.data) as ultimoEsame
FROM professori p
LEFT JOIN corsi c
ON p.matricola = c.professore
LEFT JOIN esami e
ON e.corso = c.codice
group by p.matricola
order by ultimoEsame DESC;

/*Esercizio 6*/
SELECT s.nome, s.cognome, AVG(e.voto) as Media
FROM studenti s
INNER JOIN esami e
ON e.studente = s.matricola
WHERE matricola LIKE 'IN%'
GROUP BY e.studente;

/*Esercizio 7*/
SELECT s.nome, s.cognome, SUM(e.voto * c.cfu)/SUM(c.cfu)
FROM studenti s
INNER JOIN esami e
ON e.studente = s.matricola
INNER JOIN corsi c
ON e.corso = c.codice
WHERE matricola LIKE 'IN%'
GROUP BY e.studente;

/*Esercizio 8*/
SELECT c.nome as NomeCorso, year(e.data) as anno,
       count(e.studente) as n_esami, max(e.voto) as voto_max, sum(e.lode) as n_lodi
FROM esami e
INNER JOIN corsi c
ON e.corso = c.codice
WHERE e.studente LIKE 'IN%'
GROUP BY c.nome, year(e.data)
ORDER BY c.nome, anno;
