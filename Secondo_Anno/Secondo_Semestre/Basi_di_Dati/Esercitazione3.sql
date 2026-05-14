USE unidb;
-- Esercizio 10
SELECT count(*)
FROM(SELECT studente
    FROM esami
    GROUP BY studente
    HAVING sum(lode) = 0
    UNION
    SELECT matricola
    FROM studenti s LEFT OUTER JOIN esami e ON s.matricola = e.studente
    WHERE voto IS NULL) t;

SELECT count(*)
FROM studenti s
WHERE s.matricola NOT IN (SELECT studente FROM esami GROUP BY studente HAVING sum(lode) > 0);

SELECT count(*)
FROM studenti s
WHERE s.matricola NOT IN (SELECT studente FROM esami WHERE lode = 1);

SELECT count(*)
FROM studenti s
WHERE NOT EXISTS(SELECT lode FROM esami e1 WHERE e1.studente = s.matricola and lode > 0);

ALTER TABLE professori
ADD COLUMN buono BOOL DEFAULT false;

UPDATE professori
SET buono = true
WHERE matricola IN (SELECT c.professore
FROM corsi c JOIN esami e ON e.corso = c.codice
GROUP BY c.professore
HAVING sum(e.lode) > 0
);

SELECT count(*)
FROM professori
WHERE buono = true;

CREATE FUNCTION mediaPond(studente CHAR(9), data DATE) RETURNS float DETERMINISTIC
BEGIN
    DECLARE media float DEFAULT 0;
    SELECT sum(voto * cfu)/sum(cfu)
    INTO media
    FROM esami e JOIN corsi c ON e.corso = c.codice
    WHERE e.studente = studente and e.data < data;
    RETURN media;
end;

CREATE VIEW mediePerData AS
    SELECT data, studente, voto, mediaPond(studente, data) as mediaFinoQua
    FROM esami;
SELECT * FROM mediePerData;

CREATE FUNCTION contaAumenti(studente CHAR(9)) RETURNS int DETERMINISTIC
BEGIN
    DECLARE conto INT DEFAULT 0;
    SELECT count(*) INTO conto
    FROM mediePerData m
    -- se ha aumentato il voto (se voto maggiore media sicuro aumenta)
    WHERE m.studente = studente AND m.mediaFinoQua < m.voto;
    RETURN conto;
end;

SELECT s.matricola, contaAumenti(s.matricola)
FROM studenti s
ORDER BY s.matricola;

-- Esercizio 13
SELECT avg(voto)
FROM esami
GROUP BY SUBSTR(studente, 1, 4);

CREATE FUNCTION media(studente CHAR(9)) RETURNS float DETERMINISTIC
BEGIN
    DECLARE average FLOAT DEFAULT 0;
    SELECT avg(voto) INTO average
    FROM esami
    WHERE esami.studente = studente;
    RETURN average;
end;

SELECT s.nome, s.cognome
FROM studenti s
WHERE media(s.matricola) > (SELECT avg(voto) FROM esami e
                                    WHERE substr(s.matricola, 1, 4) = substr(e.studente, 1, 4));

-- Esercizio 14
CREATE FUNCTION brillato(stud CHAR(9), prof INT) RETURNS bool DETERMINISTIC
BEGIN
    DECLARE ris1 BOOL default FALSE;
    DECLARE ris2 BOOL default FALSE;
    -- Parte per vedere se ha fatto tutti i corsi del prof
    SELECT TRUE INTO ris1
    FROM studenti
    WHERE matricola IN (SELECT DISTINCT e.studente
                        FROM corsi c LEFT JOIN esami e ON e.corso = c.codice
                        WHERE c.professore = prof);
    -- Parte per vedere se ha preso piu della media
    SELECT TRUE INTO ris2
    FROM corsi c LEFT JOIN esami e ON e.corso = c.codice
    WHERE  > (SELECT avg(voto)
                                    FROM esami WHERE corso =)
    SELECT e.corso
    FROM corsi c LEFT JOIN esami e ON e.corso = c.codice
    WHERE c.professore = prof;
    SELECT avg(voto)
    FROM esami
    WHERE corso =
end;
-- 1. Dichiariamo le variabili e le forziamo (CAST) ad essere di tipo TIME
SET @ora_spaccata = CAST('21:00:00' AS TIME);
SET @ora_con_minuti = CAST('20:30:00' AS TIME);

-- 2. Testiamo cosa succede con il + 2 vs ADDTIME
SELECT
    @ora_spaccata AS Partenza,
    @ora_spaccata + 2 AS Errore_MySQL_Col_Piu_Due,
    ADDTIME(@ora_spaccata, '02:00:00') AS Corretto_Con_Addtime,
    @ora_spaccata + '02:00:00';

-- 3. Testiamo cosa succede se ci sono i minuti
SELECT
    @ora_con_minuti AS Partenza_Con_Minuti,
    @ora_con_minuti + 2 AS Errore_MySQL_Minuti,
    ADDTIME(@ora_con_minuti, '02:00:00') AS Corretto_Con_Minuti;