## UTILISER NEWINTERNALUSER ET PAS NEWUSER
## UTILISER SETUSERACCOUNTSELECTIONAUTOSELECTSHAREACCOUNT

###  ----------- CREATION DE LA BASE DE DONNES -----------
import sqlite3

conn = sqlite3.connect('links.db', check_same_thread=False)
cursor = conn.cursor()

cursor.execute('''
    CREATE TABLE IF NOT EXISTS Links (
        lien TEXT PRIMARY KEY,
        groupe TEXT,
        nb_max INTEGER,
        nb_actu INTEGER
    )            
''')

conn.commit()
###  ----------- FIN CREATION BASE DE DONNES ----------------



### ---------- FONCTION INTERACTION BASE DE DONNES -------------
def get_all_table():
    requete = 'SELECT * FROM Links'
    cursor.execute(requete)
    rows = cursor.fetchall()
    return rows

def get_group(link):
    requete = '''
        SELECT groupe
        FROM Links
        WHERE lien = ?
    '''
    cursor.execute(requete, (str(link), ))
    row = cursor.fetchall()
    return row[0][0]

def get_max_actu(link):
    requete = '''
        SELECT nb_max, nb_actu
        FROM Links
        WHERE lien = ?
    '''
    cursor.execute(requete, (str(link),))
    row = cursor.fetchall()
    return row[0]

def exists(link):
    requete = '''
        SELECT 1
        FROM Links
        WHERE lien = ?
        LIMIT 1
    '''
    cursor.execute(requete, (str(link),))
    result = cursor.fetchone()
    return result != None

def peutCreer(link):
    if (exists(link)):
        maxx, actu = get_max_actu(link)
        return actu < maxx
    return False

def inserer_lien(link, nb_max, group):
    requete = '''
        INSERT INTO Links (lien, nb_max, nb_actu, groupe) VALUES (?,?,?,?)
    '''
    cursor.execute(requete, (str(link), str(nb_max), int(0), str(group)))
    conn.commit()

def get_actu(link):
    requete = '''
        SELECT nb_actu FROM Links WHERE lien = ?
    '''
    cursor.execute(requete, (str(link),))
    row = cursor.fetchall()
    return row[0][0]

def delete_link(link):
    requete = '''
        DELETE From Links WHERE lien = ?
    '''
    cursor.execute(requete, (str(link),))
    conn.commit()

def update_link(link):
    requete = '''
        UPDATE Links SET nb_actu = ? WHERE lien = ?
    '''    
    cursor.execute(requete, (int(get_actu(link)+1), str(link)))
    conn.commit()
        
### --------- FIN FONCTION INTERACTION BASE DE DONNES -----------



### --------- FONCTION VERIF DATA ------------------
def dataOK(data):
    emailOK = bool(validate_email(data['email']))
    nomcOK = bool(re.match(r'^[a-zA-ZÀ-ÖØ-öø-ÿ\-._ ]+$', data['nomc']))
    usernameOK = bool(re.match(r'^[a-zA-ZÀ-ÖØ-öø-ÿ\-._ ]+$', data['username']))
    return emailOK and nomcOK and usernameOK
### --------- FIN FONCTION VERIF DATA --------------



### -------------- CONFIGURATION SERVEUR -----------------
from flask import Flask, request, jsonify, render_template, send_from_directory, redirect, url_for, session
from flask_login import LoginManager, UserMixin, login_user, login_required, logout_user, current_user
from email_validator import validate_email
import uuid 
import xmlrpc.client
import ssl
import re

app = Flask(__name__)
app.secret_key = ""

host = input("Donnez l'adresse ip du serveur : \n")
port = input("Donnez le port du serveur : \n")

login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'

users = {'Administrateur': {'password': ''}}

class User(UserMixin):
    def __init__(self, username):
        self.id = username

@login_manager.user_loader
def load_user(user_id):
    if user_id in users:
        return User(user_id)
    return None


server_url = ""
auth_token = ""

context = ssl._create_unverified_context()

server = xmlrpc.client.ServerProxy(server_url, context=context)
### --------------- FIN CONFIGURATION SERVEUR



### ------------ ROUTE SERVEUR ----------------------
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username in users and users[username]['password'] == password:
            user = User(username)
            login_user(user)
            return redirect(url_for('admin'))
        return 'Invalid credentials', 401
    return render_template('Admin/login.html')


@app.route('/admin')
@login_required
def admin():
    return render_template("Admin/index.html")


@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('login'))


@app.route('/generate_link', methods=['POST'])
def generate_link():
    data = request.json
    group = data['group']
    max_accounts = data['max_accounts']
    link_id = str(uuid.uuid4())
    link = f'http://{host}:{port}/create_account/{link_id}'
    inserer_lien(link, max_accounts, group)
    return jsonify({"success" : True}), 200


@app.route('/delete', methods=['POST'])
def delete():
    data = request.json 
    link = data['link']
    delete_link(link)
    return jsonify({"success" : True}), 200


@app.route('/get_list', methods=['POST'])
def get_list():
    result = server.api.listSharedAccounts(auth_token, 0, 1000)
    if (result):
        return jsonify({"success" : True, "liste" : result}), 200
    else:
        return jsonify({"success": False, "errors": "Erreur accès liste compte partager"}), 400


@app.route('/get_all', methods=['POST'])
def get_all():
    result = get_all_table()
    return jsonify({"success" : True, "liste" : result}), 200


@app.route('/create_account/<link_id>')
def create_account(link_id):
    link = f'http://{host}:{port}/create_account/{link_id}'
    if (peutCreer(link)):
        # Tu peux cree un compte
        return render_template("User/create_account.html")
    else:
        #tu peux pas creer renvoie une page 404
        return render_template("User/error404.html")


@app.route('/static/<path:path>')
def send_static(path):
    return send_from_directory('static', path)


@app.route('/submit', methods=['POST'])
def submit():
    data = request.json
    if (dataOK(data)):
        # cree compte API
        
        nomc = data['nomc']
        username = data['username']
        email = data['email']
        password = data['password']
        link = data['link']
        
        result = server.api.addNewInternalUser(auth_token, username, password, nomc, email, "", "", True)
        
        if (result):
            server.api.setUserAccountSelectionAutoSelectSharedAccount(auth_token, username, get_group(link), False)
            update_link(link)
            
            return jsonify({"success" : True}), 200
        else:
            return jsonify({"success": False, "errors": "Probleme API, l'utilisateur existe deja !"}), 400
        
    else:
        return jsonify({"success": False, "errors": "Erreur formulaire"}), 400
### ------------- FIN ROUTE SERVEUR ---------------------