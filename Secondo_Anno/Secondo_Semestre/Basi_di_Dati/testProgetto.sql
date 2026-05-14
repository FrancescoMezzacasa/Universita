USE campionato;

-- 1. TABELLE PADRE
CREATE TABLE Societa_Sportiva (
    Codice_Affiliazione VARCHAR(20) PRIMARY KEY,
    Nome VARCHAR(100),
    Indirizzo VARCHAR(150),
    Email VARCHAR(100),
    Telefono VARCHAR(20)
);

CREATE TABLE Palazzetto (
    ID_Palazzetto INT PRIMARY KEY,
    Nome VARCHAR(100),
    Indirizzo VARCHAR(150),
    Citta VARCHAR(100),
    Capienza INT
);

CREATE TABLE Arbitro (
    Codice_Fiscale VARCHAR(16) PRIMARY KEY,
    Nome VARCHAR(50),
    Cognome VARCHAR(50),
    Citta VARCHAR(100)
);

CREATE TABLE Ufficiale_di_Campo (
    Codice_Fiscale VARCHAR(16) PRIMARY KEY,
    Nome VARCHAR(50),
    Cognome VARCHAR(50),
    Citta VARCHAR(100),
    Ruolo VARCHAR(50)
);

-- 2. TABELLA SQUADRA (Dipende da Societa e Palazzetto)
CREATE TABLE Squadra (
    ID_Squadra INT PRIMARY KEY,
    Nome VARCHAR(100),
    Colore_Casa VARCHAR(50),
    Colore_Trasferta VARCHAR(50),
    Codice_Aff_Societa VARCHAR(20),
    ID_Palazzetto INT,
    FOREIGN KEY (Codice_Aff_Societa) REFERENCES Societa_Sportiva(Codice_Affiliazione),
    FOREIGN KEY (ID_Palazzetto) REFERENCES Palazzetto(ID_Palazzetto)
);

-- 3. ALLENATORI E GIOCATORI (Dipendono da Squadra)
CREATE TABLE Allenatore (
    Codice_Fiscale VARCHAR(16) PRIMARY KEY,
    Nome VARCHAR(50),
    Cognome VARCHAR(50),
    Data_Nascita DATE,
    Livello VARCHAR(50),
    ID_Squadra INT,
    FOREIGN KEY (ID_Squadra) REFERENCES Squadra(ID_Squadra)
);

CREATE TABLE Giocatore (
    Codice_Fiscale VARCHAR(16) PRIMARY KEY,
    Nome VARCHAR(50),
    Cognome VARCHAR(50),
    Data_Nascita DATE,
    Scadenza_Certificato DATE,
    Altezza INT,
    Ruolo VARCHAR(50),
    Numero INT,
    Anno_Inizio_Militanza INT,
    ID_Squadra INT,
    FOREIGN KEY (ID_Squadra) REFERENCES Squadra(ID_Squadra)
);

-- 4. PARTITA (Dipende da Squadra e Palazzetto)
CREATE TABLE Partita (
    Codice_Partita INT PRIMARY KEY,
    Orario TIME,
    Data DATE,
    Punti_Sq_Casa INT,
    Punti_Sq_Trasferta INT,
    Giornata INT,
    ID_Palazzetto INT,
    ID_Squadra_Casa INT,
    ID_Squadra_Trasferta INT,
    FOREIGN KEY (ID_Palazzetto) REFERENCES Palazzetto(ID_Palazzetto),
    FOREIGN KEY (ID_Squadra_Casa) REFERENCES Squadra(ID_Squadra),
    FOREIGN KEY (ID_Squadra_Trasferta) REFERENCES Squadra(ID_Squadra)
);

-- 5. TABELLE DI ASSOCIAZIONE N:N
CREATE TABLE Tabellino (
    CF_Giocatore VARCHAR(16),
    Codice_Partita INT,
    Punti INT,
    Rimbalzi INT,
    Tiri_da_3 INT,
    Falli_Tecnici INT,
    Espulsione BOOLEAN,
    Giornate_Squalifica INT,
    PRIMARY KEY (CF_Giocatore, Codice_Partita),
    FOREIGN KEY (CF_Giocatore) REFERENCES Giocatore(Codice_Fiscale),
    FOREIGN KEY (Codice_Partita) REFERENCES Partita(Codice_Partita)
);

CREATE TABLE Direzione (
    CF_Arbitro VARCHAR(16),
    Codice_Partita INT,
    PRIMARY KEY (CF_Arbitro, Codice_Partita),
    FOREIGN KEY (CF_Arbitro) REFERENCES Arbitro(Codice_Fiscale),
    FOREIGN KEY (Codice_Partita) REFERENCES Partita(Codice_Partita)
);

CREATE TABLE Tavolo (
    CF_UdC VARCHAR(16),
    Codice_Partita INT,
    PRIMARY KEY (CF_UdC, Codice_Partita),
    FOREIGN KEY (CF_UdC) REFERENCES Ufficiale_di_Campo(Codice_Fiscale),
    FOREIGN KEY (Codice_Partita) REFERENCES Partita(Codice_Partita)
);

-- Inserimento Società e Palazzetti
INSERT INTO Societa_Sportiva VALUES
('SOC_MIL', 'Olimpia Milano', 'Via Olimpia 1', 'info@olimpia.it', '02123456'),
('SOC_BOL', 'Virtus Bologna', 'Via Virtus 2', 'info@virtus.it', '05123456');

INSERT INTO Palazzetto VALUES
(1, 'Forum', 'Via Milanofiori', 'Milano', 12000),
(2, 'PalaDozza', 'Piazza Azzarita', 'Bologna', 5000);

-- Inserimento Squadre
INSERT INTO Squadra VALUES
(10, 'Olimpia Milano Basket', 'Rosso', 'Bianco', 'SOC_MIL', 1),
(20, 'Virtus Segafredo Bologna', 'Nero', 'Bianco', 'SOC_BOL', 2);

-- Inserimento Arbitri e UdC (Ne mettiamo uno di Roma neutrale, uno di Milano e uno di Bologna)
INSERT INTO Arbitro VALUES
('ARB_RM01', 'Mario', 'Rossi', 'Roma'),
('ARB_MI01', 'Luigi', 'Verdi', 'Milano');

INSERT INTO Ufficiale_di_Campo VALUES
('UDC_BO01', 'Carlo', 'Neri', 'Bologna', 'Segnapunti');

-- Inserimento Giocatori (Certificati: alcuni validi, uno scade tra poco, uno già scaduto nel 2022)
INSERT INTO Giocatore VALUES
('GIO_MIL_01', 'Nicolò', 'Melli', '1991-01-26', '2026-12-31', 205, 'Ala', 9, 2021, 10),
('GIO_MIL_02', 'Shavon', 'Shields', '1994-06-05', CURRENT_DATE + INTERVAL 15 DAY, 201, 'Guardia', 31, 2020, 10), -- Scade a breve per la Query 4
('GIO_BOL_01', 'Marco', 'Belinelli', '1986-03-25', '2026-12-31', 196, 'Guardia', 3, 2020, 20),
('GIO_BOL_ERR', 'Tizio', 'Caio', '2000-01-01', '2022-01-01', 190, 'Play', 5, 2021, 20); -- Certificato scaduto per testare Trigger 4

-- Inserimento Partita 1 (Giornata 1: Milano - Bologna a Milano)
-- Inizia alle 20:00 del 1 Ottobre 2023.
INSERT INTO Partita VALUES
(100, '20:00:00', '2023-10-01', 85, 80, 1, 1, 10, 20);

-- Inserimento Tabellini per Partita 1 (Così testiamo l'MVP)
INSERT INTO Tabellino VALUES
('GIO_MIL_01', 100, 20, 10, 2, 0, FALSE, 0), -- MVP potenziale
('GIO_MIL_02', 100, 15, 5, 3, 1, FALSE, 0),
('GIO_BOL_01', 100, 25, 2, 5, 0, FALSE, 0);

-- Inseriamo l'arbitro neutrale di Roma per la Partita 1
INSERT INTO Direzione VALUES ('ARB_RM01', 100);

SELECT * FROM Squadra;

-- TEST TRIGGER 1 (Neutralità Arbitro)
DROP TRIGGER IF EXISTS ControlloArbitriUdC;
DELIMITER $$
CREATE TRIGGER ControlloArbitriUdC
BEFORE INSERT ON Direzione
FOR EACH ROW
BEGIN
DECLARE cittaArbitro VARCHAR(100);
DECLARE cittaCasa VARCHAR(100);
DECLARE cittaTrasf VARCHAR(100);
-- Cerco la città dell’arbitro
SELECT Citta INTO cittaArbitro
FROM Arbitro
WHERE NEW.CF_Arbitro = Codice_Fiscale;
-- Cerco la città della squadra di casa (tramite il palazzetto)
SELECT Citta INTO cittaCasa
FROM Partita p
JOIN Squadra s ON p.ID_Squadra_Casa = s.ID_Squadra
JOIN Palazzetto pal ON s.ID_Palazzetto = pal.ID_Palazzetto
WHERE NEW.Codice_Partita = p.Codice_Partita;
-- Cerco la città della squadra in trasferta
SELECT Citta INTO cittaTrasf
FROM Partita p
JOIN Squadra s ON p.ID_Squadra_Trasferta = s.ID_Squadra
JOIN Palazzetto pal ON s.ID_Palazzetto = pal.ID_Palazzetto
WHERE NEW.Codice_Partita = p.Codice_Partita;
-- Faccio il controllo
IF cittaArbitro = cittaCasa OR cittaArbitro = cittaTrasf THEN
	SIGNAL sqlstate "45001" SET message_text = "Conflitto di interessi!";
END IF;
END $$
DELIMITER ;
-- Provo ad assegnare l'arbitro di Milano ('ARB_MI01') alla Partita 100,
-- che si gioca al Forum di Milano, tra Milano e Bologna.
-- DEVE FALLIRE dando "Conflitto di interessi!".
INSERT INTO Direzione VALUES ('ARB_MI01', 100);


-- TEST TRIGGER 2 (Sovrapposizione Orari)
DELIMITER $$
CREATE TRIGGER controlloOrari
BEFORE INSERT ON Partita
FOR EACH ROW
BEGIN
DECLARE contoSovrapposizioni INT DEFAULT 0;
SELECT COUNT(*) INTO contoSovrapposizioni
FROM Partita p
WHERE p.ID_Palazzetto = NEW.ID_Palazzetto AND p.Data = NEW.Data AND
((NEW.Orario > p.Orario AND NEW.Orario < ADDTIME(p.Orario,"02:00:00")) OR
(ADDTIME(NEW.Orario, "02:00:00") > p.Orario AND
ADDTIME(NEW.Orario, "02:00:00") < ADDTIME(p.Orario, "02:00:00")));
/*Sto controllando se l’orario d’inizio della nuova partita		                  (NEW.Orario) è compreso tra l’inizio e la fine di una 				partita già esistente, oppure se la fine della nuova 				partita (ADDTIME(NEW.Orario, ‘02:00:00’)) è compreso tra 				l’inizio e la fine di una partita già esistente*/
IF contoSovrapposizioni > 0 THEN
	SIGNAL sqlstate "45002" SET message_text = "Partite sovrapposte!";
END IF;
END $$
DELIMITER ;

-- Provo a creare una nuova partita al Forum (ID 1), stessa data (2023-10-01),
-- alle ore 21:00. La Partita 100 inizia alle 20:00, quindi i due orari si accavallano!
-- DEVE FALLIRE dando "Partite sovrapposte!".
INSERT INTO Partita VALUES
(101, '21:00:00', '2023-10-01', 0, 0, 1, 1, 20, 10);


-- TEST TRIGGER 3 (Coerenza Squalifiche)
DELIMITER $$
CREATE TRIGGER controlloGiornatediSqualifica
BEFORE INSERT ON Tabellino
FOR EACH ROW
BEGIN
IF NEW.Giornate_Squalifica > 0 AND NEW.Espulsione = FALSE THEN
	SIGNAL sqlstate "45003" SET message_text = "Errore sanzioni!";
END IF;
END $$
DELIMITER ;
-- Inserisco un nuovo tabellino per la Partita 100. Assegno 2 giornate di squalifica,
-- ma metto Espulsione a FALSE.
-- DEVE FALLIRE dando "Errore sanzioni!".
INSERT INTO Tabellino VALUES
('GIO_BOL_01', 100, 0, 0, 0, 0, FALSE, 2);


-- TEST TRIGGER 4 (Certificato Medico)
DELIMITER $$
CREATE TRIGGER controlloCertificato
BEFORE INSERT ON Tabellino
FOR EACH ROW
BEGIN
DECLARE scadCert DATE;
DECLARE dataPartita DATE;
-- Ottengo la data di scadenza del certificato
SELECT Scadenza_Certificato INTO scadCert
FROM Giocatore
WHERE Codice_Fiscale = NEW.CF_Giocatore;
-- Ottengo la data della partita
SELECT Data INTO dataPartita
FROM Partita
WHERE Codice_Partita = NEW.Codice_Partita;
-- Faccio il controllo
IF scadCert < dataPartita THEN
    SIGNAL sqlstate "45004" SET message_text = "Certificato scaduto!";
END IF;
END $$
DELIMITER ;

-- Provo a far giocare il giocatore 'GIO_BOL_ERR' (che ha il certificato scaduto nel 2022)
-- inserendolo nel tabellino della Partita 100 (giocata nel 2023).
-- DEVE FALLIRE dando "Certificato scaduto!".
INSERT INTO Tabellino VALUES
('GIO_BOL_ERR', 100, 10, 2, 1, 0, FALSE, 0);

CREATE INDEX indiceGiornata ON Partita(Giornata);

-- 1. Aggiungiamo altre due Società e i rispettivi Palazzetti
INSERT INTO Societa_Sportiva VALUES
('SOC_VAR', 'Pallacanestro Varese', 'Piazzale Gramsci', 'info@pallacanestrovarese.it', '0332111'),
('SOC_VEN', 'Reyer Venezia', 'Via Taliercio', 'info@reyer.it', '0412222');

INSERT INTO Palazzetto VALUES
(3, 'Palasport Lino Oldrini', 'Piazzale Gramsci', 'Varese', 5100),
(4, 'Palasport Taliercio', 'Via Vendramin', 'Venezia', 3500);

-- 2. Aggiungiamo le Squadre
INSERT INTO Squadra VALUES
(30, 'Openjobmetis Varese', 'Rosso', 'Bianco', 'SOC_VAR', 3),
(40, 'Umana Reyer Venezia', 'Oro', 'Granata', 'SOC_VEN', 4);

-- 3. Aggiungiamo qualche Arbitro "neutrale" in più per coprire le partite
INSERT INTO Arbitro VALUES
('ARB_TO01', 'Giovanni', 'Bianchi', 'Torino'),
('ARB_FI01', 'Andrea', 'Gialli', 'Firenze');

-- 4. Aggiungiamo i Giocatori (Con certificati tutti validi per evitare blocchi dal trigger)
INSERT INTO Giocatore VALUES
('GIO_VAR_01', 'Nico', 'Mannion', '2001-03-14', '2026-12-31', 190, 'Play', 1, 2023, 30),
('GIO_VAR_02', 'Davide', 'Moretti', '1998-03-25', '2026-12-31', 190, 'Guardia', 11, 2023, 30),
('GIO_VEN_01', 'Marco', 'Spissu', '1995-02-05', '2026-12-31', 184, 'Play', 0, 2022, 40),
('GIO_VEN_02', 'Amedeo', 'Tessitori', '1994-10-07', '2026-12-31', 208, 'Centro', 0, 2022, 40);


-- 5. IL CALENDARIO: Creiamo le altre partite!
-- Abbiamo già la Partita 100 (Milano - Bologna, vinta da Milano 85-80) in Giornata 1.

-- Completiamo la Giornata 1: Varese - Venezia
INSERT INTO Partita VALUES
(101, '20:30:00', '2023-10-01', 90, 95, 1, 3, 30, 40); -- Vince Venezia in trasferta

-- Giochiamo tutta la Giornata 2:
INSERT INTO Partita VALUES
(102, '18:15:00', '2023-10-08', 100, 85, 2, 2, 20, 30), -- Bologna - Varese (Vince Bologna)
(103, '19:00:00', '2023-10-08', 80, 88, 2, 4, 40, 10); -- Venezia - Milano (Vince Milano)


-- 6. LE STATISTICHE: I Tabellini per l'MVP
-- Statistiche per la Partita 101 (Varese-Venezia)
INSERT INTO Tabellino VALUES
('GIO_VAR_01', 101, 25, 3, 4, 1, FALSE, 0), -- Mannion fa tanti punti, ma prende 1 tecnico
('GIO_VEN_01', 101, 30, 5, 6, 0, FALSE, 0); -- Spissu scatenato, 0 tecnici

-- Statistiche per la Partita 102 (Bologna-Varese)
INSERT INTO Tabellino VALUES
('GIO_BOL_01', 102, 22, 2, 5, 0, FALSE, 0),
('GIO_VAR_01', 102, 18, 5, 2, 2, FALSE, 0); -- Mannion prende altri 2 falli tecnici (perde colpi per l'MVP!)

-- Statistiche per la Partita 103 (Venezia-Milano)
INSERT INTO Tabellino VALUES
('GIO_VEN_01', 103, 15, 4, 3, 0, FALSE, 0),
('GIO_MIL_01', 103, 18, 12, 1, 0, FALSE, 0); -- Melli (che aveva già giocato la 100) fa una doppia-doppia

SELECT * FROM Squadra;

DELIMITER $$
CREATE PROCEDURE stampaCalendarioGiornata(IN giorn INT)
BEGIN
	SELECT sc.Nome AS SquadraDiCasa, st.Nome AS SquadraInTrasferta, pal.Indirizzo, p.Data, p.Orario
	FROM Partita p
	JOIN Palazzetto pal USING(ID_Palazzetto)
	JOIN Squadra sc ON p.ID_Squadra_Casa = sc.ID_Squadra
	JOIN Squadra st ON p.ID_Squadra_Trasferta = st.ID_Squadra
	WHERE p.giornata = giorn;
END $$
DELIMITER ;

CALL stampaCalendarioGiornata(2);

DELIMITER $$
CREATE FUNCTION calcolaPunti(puntiSq1 INT, puntiSq2 INT) RETURNS INT DETERMINISTIC
BEGIN
	IF puntiSq1 > puntiSq2 THEN
		return 2;
	ELSE
		RETURN 0;
	END IF;
END $$
DELIMITER ;

DELIMITER $$
CREATE PROCEDURE mostraClassifica(IN giornataLimite INT)
BEGIN
	-- Conto prima quante ne hanno vinte in casa e poi in trasferta
	SELECT nomeSquadra, SUM(puntiPartita) as PuntiTotali
	FROM (SELECT s.nome AS nomeSquadra, calcolaPunti(p.Punti_Sq_Casa, 				p.Punti_Sq_Trasferta) AS puntiPartita
		FROM Partita p
		JOIN Squadra s ON p.ID_Squadra_Casa = s.ID_Squadra
		WHERE p.Giornata <= giornataLimite
		UNION ALL
		SELECT s.nome AS nomeSquadra, calcolaPunti(p.Punti_Sq_Trasferta, 			p.Punti_Sq_Casa) AS puntiPartita
		FROM Partita p
		JOIN Squadra s ON p.ID_Squadra_Trasferta = s.ID_Squadra
		WHERE p.Giornata <= giornataLimite) punteggiSingoli
	GROUP BY nomeSquadra
	ORDER BY PuntiTotali DESC;
END $$
DELIMITER ;

CALL mostraClassifica(2);

DELIMITER $$
CREATE PROCEDURE nominaMVP()
BEGIN
	SELECT g.Nome, g.Cognome, (SUM(t.Punti)+SUM(t.Rimbalzi)-SUM(t.Falli_Tecnici)) as MVPscore
	FROM Tabellino t
	JOIN Giocatore g ON t.CF_Giocatore = g.Codice_Fiscale
	GROUP BY t.CF_Giocatore, g.Nome, g.Cognome
	ORDER BY MVPscore DESC
	LIMIT 1;
END $$
DELIMITER ;

CALL nominaMVP();

DELIMITER $$
CREATE PROCEDURE mostraCertificatiinScadenza()
BEGIN
	SELECT g.Nome, g.Cognome
	FROM Giocatore g
	WHERE g.Scadenza_Certificato BETWEEN CURDATE() AND
		DATE_ADD(CURDATE(), INTERVAL 1 MONTH);
END $$
DELIMITER ;

CALL mostraCertificatiinScadenza();