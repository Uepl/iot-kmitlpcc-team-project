from flask import Flask, render_template, request, redirect, url_for
import sqlite3

app = Flask(__name__)

DATABASE = './music_streaming_platform_final.db'

def get_db_connection():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row 
    return conn

def init_db():
    conn = get_db_connection()
    cursor = conn.cursor()
    with app.open_resource('./music_streaming_platform_final.sql', mode='r') as f:
        cursor.executescript(f.read())
    conn.commit()
    conn.close()

@app.route('/')
def index():
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('''
                    SELECT
                        a.id AS ArtistId,
                        a.name AS ArtistName,
                        r.name AS RecordLabel
                    FROM
                        artists AS a
                    JOIN
                        records AS r ON a.record = r.id;
                   ''')
    artists = cursor.fetchall()
    conn.close()
    return render_template('index.html', artists=artists)

# ------------------artist---------------------------------------------------------------
@app.route('/artist/<int:artist_id>/tracks')
def artistTrack(artist_id):
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('''
        SELECT
            t.id AS TrackID,
            a.name AS ArtistName,
            al.name AS AlbumName,
            t.name AS TrackName,
            t.duration AS TrackLength
        FROM
            artists AS a
        JOIN
            albums AS al ON a.id = al.artist
        JOIN
            tracks AS t ON al.id = t.album
        WHERE
            a.id = ?;
    ''', (artist_id,))
    tracks = cursor.fetchall()
    artist_name = tracks[0]['ArtistName'] if tracks else 'Unknown Artist'
    conn.close()
    return render_template('artistTrack.html', tracks=tracks, artist_id=artist_id, artist_name=artist_name)

@app.route('/artists/add', methods=['GET', 'POST'])
def add_artist():
    if request.method == 'POST':
        name = request.form['name']
        record = request.form['record']
        conn = get_db_connection()
        conn.execute('INSERT INTO artists (name, record) VALUES (?, ?)', (name, record))
        conn.commit()
        conn.close()
        return redirect(url_for('index'))
    return render_template('add_artist.html')

@app.route('/artists/delete/<int:artist_id>')
def delete_artist(artist_id):
    conn = get_db_connection()
    conn.execute('DELETE FROM artists WHERE id = ?', (artist_id,))
    conn.commit()
    conn.close()
    return redirect(url_for('index'))

@app.route('/artists/edit/<int:artist_id>', methods=['GET', 'POST'])
def edit_artist(artist_id):
    conn = get_db_connection()
    artist = conn.execute('SELECT * FROM artists WHERE id = ?', (artist_id,)).fetchone()

    if request.method == 'POST':
        name = request.form['name']
        record = request.form['record']
        conn.execute('UPDATE artists SET name = ?, record = ? WHERE id = ?', (name, record, artist_id))
        conn.commit()
        conn.close()
        return redirect(url_for('index'))

    conn.close()
    return render_template('editArtist.html', artist=artist)

#---------------------------------------------------------------------------------

#------------------track------------------------------------------------------
@app.route('/track/<int:track_id>')
def track_details(track_id):
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('''
        SELECT
            t.id,
            t.name AS TrackName,
            al.name AS AlbumName,
            a.name AS ArtistName,
            a.id As ArtistId
        FROM
            tracks AS t
        JOIN
            albums AS al ON t.album = al.id
        JOIN
            artists AS a ON al.artist = a.id
        WHERE
            t.id = ?;
    ''', (track_id,))
    track = cursor.fetchone()
    conn.close()
    if track:
        return render_template('trackDetails.html', track=track)
    else:
        return "Track not found", 404

@app.route('/track/delete/<int:track_id>')
def delete_track(track_id):
    conn = get_db_connection()
    conn.execute('DELETE FROM tracks WHERE id = ?', (track_id,))
    conn.commit()
    conn.close()
    return redirect(url_for('index'))

@app.route('/track/edit/<int:track_id>', methods=['GET','POST'])
def editTrack(track_id):
    conn = get_db_connection()
    track = conn.execute('SELECT * FROM tracks WHERE id = ?', (track_id,)).fetchone()
    albums = conn.execute('SELECT id, name FROM albums').fetchall() 
    if request.method == 'POST':
        name = request.form['name']
        duration = request.form['duration']
        album_id = request.form['album_id']
        play_count = request.form['play_count']
        release_date = request.form['release_date']

        conn.execute('UPDATE tracks SET name = ?, duration = ?, album = ?, play_count = ?, release_date = ? WHERE id = ?',
                     (name, duration, album_id, play_count, release_date, track_id))
        conn.commit()
        conn.close()
        return redirect(url_for('track_details', track_id=track_id))

    conn.close()
    return render_template('editTrack.html', track=track, albums=albums)

#---------------------------------------------------------------------------------

#------------------record---------------------------------------------------------------
@app.route('/records')
def records():
    conn = get_db_connection()
    records = conn.execute('SELECT * FROM records').fetchall()
    conn.close()
    return render_template('records.html', records=records)

@app.route('/records/edit/<int:record_id>', methods=['GET', 'POST'])
def editRecord(record_id):
    conn = get_db_connection()
    records = conn.execute('SELECT * FROM records WHERE id = ?', (record_id, )).fetchall()
    if request.method == 'POST':
        id = request.form['id']
        name = request.form['name']
        try:
            conn.execute('UPDATE records SET id = ?, name = ? WHERE id = ?', (id, name, record_id))
            conn.commit()
            conn.close()
            return redirect(url_for('records'))
        except sqlite3.IntegrityError:
            conn.close()
            return "Error: ID already exists or violates constraints.", 400
    conn.close()
    return render_template('recordsEdit.html', records=records)

#---------------------------------------------------------------------------------

if __name__ == '__main__':
    app.run(debug=True)