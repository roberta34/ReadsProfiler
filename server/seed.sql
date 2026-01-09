INSERT INTO authors (name) VALUES
('Jane Austen'),
('Nicholas Sparks'),
('Jojo Moyes'),
('Diana Gabaldon'),
('J.R.R. Tolkien'),
('George R.R. Martin'),
('Brandon Sanderson'),
('Patrick Rothfuss'),
('Frank Herbert'),
('Isaac Asimov'),
('Philip K. Dick'),
('Arthur C. Clarke'),
('Stephen King'),
('Bram Stoker'),
('Mary Shelley'),
('Harper Lee'),
('Markus Zusak'),
('F. Scott Fitzgerald'),
('Fyodor Dostoevsky'),
('John Steinbeck'),
('Robert Louis Stevenson'),
('Jules Verne'),
('Jack London'),
('Tom Clancy'),
('Dan Brown'),
('George Orwell'),
('Aldous Huxley'),
('Ray Bradbury'),
('Suzanne Collins'),
('Yuval Noah Harari');

INSERT INTO genres (name) VALUES
('Romance'),
('Fantasy'),
('Science Fiction'),
('Horror'),
('Family'),
('Drama'),
('Adventure'),
('Action'),
('Dystopia'),
('History');

INSERT INTO genres (name, parent_id) VALUES
('Contemporary Romance', 1),
('Historical Romance', 1),

('Epic Fantasy', 2),
('Dark Fantasy', 2),

('Space Opera', 3),
('Cyberpunk', 3),

('Psychological Horror', 4),
('Gothic Horror', 4),

('Coming of Age', 5),

('Psychological Drama', 6),

('Exploration', 7),

('Spy Thriller', 8),

('Totalitarian Society', 9),

('Modern History', 10);

--ROMANCE--
INSERT INTO books (title, author_id, year, ISBN, rating, file_path) VALUES
('Pride and Prejudice', 1, 1813, 'ISBN0001', 4.8, '/books/prideandprejudice.txt'),
('Sense and Sensibility', 1, 1811, 'ISBN0002', 4.6, '/books/senseandsensibility.txt'),
('Emma', 1, 1815, 'ISBN0003', 4.5, '/books/emma.txt'),
('The Notebook', 2, 1996, 'ISBN0004', 4.6, '/books/thenotebook.txt'),
('A Walk to Remember', 2, 1999, 'ISBN0005', 4.4, '/books/awalktoremember.txt'),
('Me Before You', 3, 2012, 'ISBN0006', 4.5, '/books/mebeforeyou.txt'),
('After You', 3, 2015, 'ISBN0007', 4.3, '/books/afteryou.txt'),
('Outlander', 4, 1991, 'ISBN0008', 4.7, '/books/outlander.txt'),
('The Time Traveler''s Wife', 3, 2003, 'ISBN0009', 4.4, '/books/timetravelerswife.txt'),
('One Day', 2, 2009, 'ISBN0010', 4.2, '/books/oneday.txt');

--FANTASY--
INSERT INTO books VALUES
(NULL,'The Hobbit',5,1937,'ISBN0011',4.9,'/books/thehobbit.txt'),
(NULL,'The Lord of the Rings',5,1954,'ISBN0012',5.0,'/books/thelordoftherings.txt'),
(NULL,'The Silmarillion',5,1977,'ISBN0013',4.7,'/books/silmarillion.txt'),
(NULL,'A Game of Thrones',6,1996,'ISBN0014',4.8,'/books/gameofthrones.txt'),
(NULL,'A Clash of Kings',6,1998,'ISBN0015',4.7,'/books/clashofkings.txt'),
(NULL,'Mistborn',7,2006,'ISBN0016',4.7,'/books/mistborn.txt'),
(NULL,'The Way of Kings',7,2010,'ISBN0017',4.9,'/books/wayofkings.txt'),
(NULL,'The Name of the Wind',8,2007,'ISBN0018',4.8,'/books/nameofthewind.txt'),
(NULL,'The Wise Man''s Fear',8,2011,'ISBN0019',4.6,'/books/wisemansfear.txt'),
(NULL,'American Gods',12,2001,'ISBN0020',4.5,'/books/americangods.txt');

--SCIENCE-FICTION--
INSERT INTO books VALUES
(NULL,'Dune',9,1965,'ISBN0021',4.9,'/books/dune.txt'),
(NULL,'Dune Messiah',9,1969,'ISBN0022',4.5,'/books/dunemessiah.txt'),
(NULL,'Foundation',10,1951,'ISBN0023',4.8,'/books/foundation.txt'),
(NULL,'Foundation and Empire',10,1952,'ISBN0024',4.7,'/books/foundationempire.txt'),
(NULL,'Neuromancer',11,1984,'ISBN0025',4.6,'/books/neuromancer.txt'),
(NULL,'Do Androids Dream of Electric Sheep?',11,1968,'ISBN0026',4.6,'/books/androids.txt'),
(NULL,'2001: A Space Odyssey',12,1968,'ISBN0027',4.7,'/books/2001.txt'),
(NULL,'Childhood''s End',12,1953,'ISBN0028',4.6,'/books/childhoodsend.txt'),
(NULL,'Ender''s Game',10,1985,'ISBN0029',4.7,'/books/endersgame.txt'),
(NULL,'Snow Crash',11,1992,'ISBN0030',4.6,'/books/snowcrash.txt');

--HORROR--
INSERT INTO books VALUES
(NULL,'It',13,1986,'ISBN0031',4.5,'/books/it.txt'),
(NULL,'The Shining',13,1977,'ISBN0032',4.6,'/books/theshining.txt'),
(NULL,'Pet Sematary',13,1983,'ISBN0033',4.5,'/books/petsematary.txt'),
(NULL,'Carrie',13,1974,'ISBN0034',4.4,'/books/carrie.txt'),
(NULL,'Dracula',14,1897,'ISBN0035',4.6,'/books/dracula.txt'),
(NULL,'Frankenstein',15,1818,'ISBN0036',4.4,'/books/frankenstein.txt'),
(NULL,'The Haunting of Hill House',14,1959,'ISBN0037',4.4,'/books/hillhouse.txt'),
(NULL,'The Exorcist',13,1971,'ISBN0038',4.5,'/books/exorcist.txt'),
(NULL,'World War Z',13,2006,'ISBN0039',4.3,'/books/worldwarz.txt'),
(NULL,'Bird Box',13,2014,'ISBN0040',4.2,'/books/birdbox.txt');

--FAMILY--
INSERT INTO books VALUES
(NULL,'To Kill a Mockingbird',16,1960,'ISBN0041',4.9,'/books/mockingbird.txt'),
(NULL,'The Book Thief',17,2005,'ISBN0042',4.8,'/books/bookthief.txt'),
(NULL,'Little Women',16,1868,'ISBN0043',4.8,'/books/littlewomen.txt'),
(NULL,'Wonder',17,2012,'ISBN0044',4.7,'/books/wonder.txt'),
(NULL,'Anne of Green Gables',17,1908,'ISBN0045',4.6,'/books/anne.txt'),
(NULL,'Matilda',17,1988,'ISBN0046',4.6,'/books/matilda.txt'),
(NULL,'Room',17,2010,'ISBN0047',4.5,'/books/room.txt'),
(NULL,'A Tree Grows in Brooklyn',17,1943,'ISBN0048',4.6,'/books/treebrooklyn.txt'),
(NULL,'The Glass Castle',17,2005,'ISBN0049',4.5,'/books/glasscastle.txt'),
(NULL,'Life of Pi',17,2001,'ISBN0050',4.5,'/books/lifeofpi.txt');

--DRAMA--
INSERT INTO books VALUES
(NULL,'The Great Gatsby',18,1925,'ISBN0051',4.4,'/books/gatsby.txt'),
(NULL,'Crime and Punishment',19,1866,'ISBN0052',4.7,'/books/crime.txt'),
(NULL,'Of Mice and Men',20,1937,'ISBN0053',4.6,'/books/ofmiceandmen.txt'),
(NULL,'The Kite Runner',18,2003,'ISBN0054',4.8,'/books/kiterunner.txt'),
(NULL,'Atonement',18,2001,'ISBN0055',4.6,'/books/atonement.txt'),
(NULL,'Beloved',18,1987,'ISBN0056',4.5,'/books/beloved.txt'),
(NULL,'The Road',18,2006,'ISBN0057',4.6,'/books/theroad.txt'),
(NULL,'Requiem for a Dream',18,1978,'ISBN0058',4.4,'/books/requiem.txt'),
(NULL,'The Catcher in the Rye',18,1951,'ISBN0059',4.3,'/books/catcher.txt'),
(NULL,'The Color Purple',18,1982,'ISBN0060',4.6,'/books/colorpurple.txt');

--ADVENTURE--
INSERT INTO books VALUES
(NULL,'Treasure Island',21,1883,'ISBN0061',4.3,'/books/treasureisland.txt'),
(NULL,'Robinson Crusoe',22,1719,'ISBN0062',4.3,'/books/robinsoncrusoe.txt'),
(NULL,'The Call of the Wild',23,1903,'ISBN0063',4.5,'/books/callofthewild.txt'),
(NULL,'Into the Wild',23,1996,'ISBN0064',4.4,'/books/intothewild.txt'),
(NULL,'The Odyssey',21,800,'ISBN0065',4.6,'/books/odyssey.txt'),
(NULL,'Around the World in 80 Days',22,1873,'ISBN0066',4.5,'/books/80days.txt'),
(NULL,'The Lost World',21,1912,'ISBN0067',4.4,'/books/lostworld.txt'),
(NULL,'Hatchet',23,1987,'ISBN0068',4.4,'/books/hatchet.txt'),
(NULL,'King Solomon''s Mines',21,1885,'ISBN0069',4.3,'/books/solomon.txt'),
(NULL,'Journey to the Center of the Earth',22,1864,'ISBN0070',4.5,'/books/centerofearth.txt');

--ACTION--
INSERT INTO books VALUES
(NULL,'The Hunt for Red October',24,1984,'ISBN0071',4.6,'/books/redoctober.txt'),
(NULL,'Patriot Games',24,1987,'ISBN0072',4.5,'/books/patriotgames.txt'),
(NULL,'Clear and Present Danger',24,1989,'ISBN0073',4.5,'/books/clearpresentdanger.txt'),
(NULL,'The Da Vinci Code',25,2003,'ISBN0074',4.4,'/books/davinci.txt'),
(NULL,'Angels and Demons',25,2000,'ISBN0075',4.3,'/books/angelsdemons.txt'),
(NULL,'Inferno',25,2013,'ISBN0076',4.3,'/books/inferno.txt'),
(NULL,'Digital Fortress',25,1998,'ISBN0077',4.2,'/books/digitalfortress.txt'),
(NULL,'The Bourne Identity',24,1980,'ISBN0078',4.6,'/books/bourne.txt'),
(NULL,'American Assassin',24,2010,'ISBN0079',4.4,'/books/americanassassin.txt'),
(NULL,'The Terminal List',24,2018,'ISBN0080',4.5,'/books/terminallist.txt');


--DISTOPIA--
INSERT INTO books VALUES
(NULL,'1984',26,1949,'ISBN0081',4.9,'/books/1984.txt'),
(NULL,'Animal Farm',26,1945,'ISBN0082',4.7,'/books/animalfarm.txt'),
(NULL,'Brave New World',27,1932,'ISBN0083',4.8,'/books/bravenewworld.txt'),
(NULL,'Fahrenheit 451',28,1953,'ISBN0084',4.7,'/books/fahrenheit451.txt'),
(NULL,'The Hunger Games',29,2008,'ISBN0085',4.6,'/books/hungergames.txt'),
(NULL,'Catching Fire',29,2009,'ISBN0086',4.5,'/books/catchingfire.txt'),
(NULL,'Mockingjay',29,2010,'ISBN0087',4.4,'/books/mockingjay.txt'),
(NULL,'The Handmaid''s Tale',27,1985,'ISBN0088',4.6,'/books/handmaid.txt'),
(NULL,'The Giver',29,1993,'ISBN0089',4.5,'/books/giver.txt'),
(NULL,'Station Eleven',27,2014,'ISBN0090',4.4,'/books/stationeleven.txt');

--HISTORY--
INSERT INTO books VALUES
(NULL,'Sapiens',30,2011,'ISBN0091',4.8,'/books/sapiens.txt'),
(NULL,'Homo Deus',30,2015,'ISBN0092',4.7,'/books/homodeus.txt'),
(NULL,'21 Lessons for the 21st Century',30,2018,'ISBN0093',4.6,'/books/21lessons.txt'),
(NULL,'Guns, Germs, and Steel',30,1997,'ISBN0094',4.6,'/books/guns.txt'),
(NULL,'The Silk Roads',30,2015,'ISBN0095',4.5,'/books/silkroads.txt'),
(NULL,'SPQR',30,2016,'ISBN0096',4.4,'/books/spqr.txt'),
(NULL,'A Brief History of Time',12,1988,'ISBN0097',4.6,'/books/briefhistory.txt'),
(NULL,'The Diary of a Young Girl',30,1947,'ISBN0098',4.8,'/books/annefrank.txt'),
(NULL,'Team of Rivals',30,2005,'ISBN0099',4.5,'/books/teamofrivals.txt'),
(NULL,'The Cold War',30,2007,'ISBN0100',4.4,'/books/coldwar.txt');

INSERT INTO book_genres (book_id, genre_id)
SELECT id,
CASE
  WHEN id BETWEEN 1 AND 10 THEN 1
  WHEN id BETWEEN 11 AND 20 THEN 2
  WHEN id BETWEEN 21 AND 30 THEN 3
  WHEN id BETWEEN 31 AND 40 THEN 4
  WHEN id BETWEEN 41 AND 50 THEN 5
  WHEN id BETWEEN 51 AND 60 THEN 6
  WHEN id BETWEEN 61 AND 70 THEN 7
  WHEN id BETWEEN 71 AND 80 THEN 8
  WHEN id BETWEEN 81 AND 90 THEN 9
  ELSE 10
END
FROM books;

INSERT INTO author_genres (author_id, genre_id) VALUES
(1,1),(2,1),(3,1),(4,1),
(5,2),(6,2),(7,2),(8,2),
(9,3),(10,3),(11,3),(12,3),
(13,4),(14,4),(15,4),
(16,5),(17,5),
(18,6),(19,6),(20,6),
(21,7),(22,7),(23,7),
(24,8),(25,8),
(26,9),(27,9),(28,9),(29,9),
(30,10);
