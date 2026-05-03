CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(24) NOT NULL UNIQUE,
    password VARCHAR(72) NOT NULL,
    level INT DEFAULT 0,
    money INT DEFAULT 500,
    skin INT DEFAULT 221,
    job INT DEFAULT 0,
    pos_x FLOAT,
    pos_y FLOAT,
    pos_z FLOAT,
    interior INT DEFAULT 0
) ENGINE=InnoDB;