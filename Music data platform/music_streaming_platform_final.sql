DROP TABLE IF EXISTS "artists";
CREATE TABLE "artists" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
    "record" TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
    FOREIGN KEY("record") REFERENCES "records"("id")
);
DROP TABLE IF EXISTS "genres";
CREATE TABLE "genres" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "records";
CREATE TABLE "records" (
    "id"	INTEGER NOT NULL,
    "name"	TEXT NOT NULL,
    PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "composers";
CREATE TABLE "composers" (
    "id"	INTEGER NOT NULL,
    "name"	TEXT NOT NULL,
    PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "playlists";
CREATE TABLE "playlists" (
    "id"	INTEGER NOT NULL,
    "name"	TEXT NOT NULL,
    PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "albums";
CREATE TABLE "albums" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"artist"	INTEGER NOT NULL,
    "release_date" TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("artist") REFERENCES "artists"("id")
);
DROP TABLE IF EXISTS "tracks";
CREATE TABLE "tracks" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"duration"	TEXT NOT NULL,
	"album"	INTEGER NOT NULL,
	"play_count"	INTEGER NOT NULL,
    "release_date" TEXT NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("album") REFERENCES "albums"("id")
);
DROP TABLE IF EXISTS "track_genres";
CREATE TABLE "track_genres" (
    "track"	INTEGER NOT NULL,
    "genre"	INTEGER NOT NULL,
    PRIMARY KEY("track","genre"),
    FOREIGN KEY("track") REFERENCES "tracks"("id"),
    FOREIGN KEY("genre") REFERENCES "genres"("id")
);
DROP TABLE IF EXISTS "track_artists";
CREATE TABLE "track_artists" (
    "track"	INTEGER NOT NULL,
    "artist"	INTEGER NOT NULL,
    PRIMARY KEY("track","artist"),
    FOREIGN KEY("track") REFERENCES "tracks"("id"),
    FOREIGN KEY("artist") REFERENCES "artists"("id")
);
DROP TABLE IF EXISTS "track_composers";
CREATE TABLE "track_composers" (
    "track"	INTEGER NOT NULL,
    "composer"	INTEGER NOT NULL,
    PRIMARY KEY("track","composer"),
    FOREIGN KEY("track") REFERENCES "tracks"("id"),
    FOREIGN KEY("composer") REFERENCES "composers"("id")
);
DROP TABLE IF EXISTS "playlist_tracks";
CREATE TABLE "playlist_tracks" (
    "playlist"	INTEGER NOT NULL,
    "track"	INTEGER NOT NULL,
    PRIMARY KEY("playlist","track"),
    FOREIGN KEY("playlist") REFERENCES "playlists"("id"),
    FOREIGN KEY("track") REFERENCES "tracks"("id")
);