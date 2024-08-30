from waitress import serve
from main import app
from main import host
from main import port

if __name__ == '__main__':
    print(f"Votre serveur démarre sur l'adresse IP : {host} et sur le port : {port}")
    serve(app, host=host, port=port)