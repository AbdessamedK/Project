import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'package:image_picker/image_picker.dart';
import 'package:navigation/constant.dart';
import 'package:navigation/screen1.dart';
import 'package:path_provider/path_provider.dart';
import 'dart:io';
import 'package:image/image.dart' as img;
import 'dart:convert';
import 'package:http/http.dart' as http;

class CameraScreen extends StatefulWidget {
  @override
  _CameraScreenState createState() => _CameraScreenState();
}

class _CameraScreenState extends State<CameraScreen> {
  late CameraController _controller;
  late Future<void> _initializeControllerFuture;

  @override
  void initState() {
    super.initState();
    _initializeControllerFuture = initializeController();
  }

  Future<void> initializeController() async {
    final cameras = await availableCameras();
    _controller = CameraController(cameras[1], ResolutionPreset.medium);
    await _controller.initialize();
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  Future<String> _getSavePath() async {
    final directory = await getApplicationDocumentsDirectory();
    return '${directory.path}/images';
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SingleChildScrollView(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            SizedBox(
              height: 20.0,
            ),
            Text(
              'RACONTE.AI',
              style: kTempTextStyle,
            ),
            SizedBox(
              height: 20.0,
            ),
            //Caméra
            FutureBuilder<void>(
              future: _initializeControllerFuture,
              builder: (context, snapshot) {
                if (snapshot.connectionState == ConnectionState.done) {
                  return CameraPreview(_controller);
                } else {
                  return Center(child: CircularProgressIndicator());
                }
              },
            ),
            Text(
              "Quelle est ton humeur, aujourd'hui ?",
              style: kTempTextStyle,
              textAlign: TextAlign.center,
            ),
            SizedBox(
              height: 20.0,
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        backgroundColor: Colors.black,
        child: Icon(
          Icons.camera,
          color: Colors.white,
        ),
        onPressed: () async {
          try {
            await _initializeControllerFuture;
            final image = await _controller.takePicture();
            final String directoryPath = await _getSavePath();
            Directory(directoryPath).create(recursive: true);
            final String imagePath = '$directoryPath/${DateTime.now()}.jpg';
            File(imagePath)
                .writeAsBytesSync(File(image.path).readAsBytesSync());

            // File? selectedimage;
            // final pickedImage =
            //     await ImagePicker().pickImage(source: ImageSource.gallery);
            // final String imagePath = pickedImage!.path;
            // Juste avant la création de la requête
            print('Chemin de l\'image capturée : $imagePath');

            final bytes = await File(imagePath).readAsBytes();

            // Création du fichier multipart
            var imageFile = http.MultipartFile.fromBytes(
              'image', // Nom du champ dans la requête
              bytes,
              filename: 'image.jpg', // Nom du fichier (ou extension appropriée)
            );

            // Envoie des donnés au serveur
            //print("Connexion au serveur");
            // Avant l'envoi de la requête
            print('Envoi de la requête au serveur...');

            //TODO: C'EST ICI QU'ON CHANGERA L'ADRESSE IP PAR CONTRE FAUT LAISSER "/process-image"
            var request = http.MultipartRequest(
                'POST', Uri.parse('http://192.168.186.79:5000/process-image'));
            //print("Connexion au serveur");
            // Après l'envoi de la requête
            print('Réponse du serveur reçue.');

            // request.files.add(
            //     await http.MultipartFile.fromPath('image', imagePath)
            //         .timeout(Duration(seconds: 30)));
            request.files.add(imageFile);
            // var multipartFile =
            //     await http.MultipartFile.fromPath('image', imagePath);
            // if (multipartFile != null) {
            //   request.files.add(multipartFile);
            //   print('Fichier ajouté à la requête : ${multipartFile.filename}');
            // } else {
            //   print('Erreur lors de la conversion du fichier');
            // }
            print("conen");
            var response = await request.send();

            print("conen");
            //REPONSE
            print("conen");

            String responseBody = await response.stream.bytesToString();

            print("conen");
            // Avant la conversion en JSON
            print('Réponse reçue du serveur : $responseBody');

            Map<String, dynamic> data = json.decode(responseBody);

            print("Récupération des textes renvoyés par le serveur");
            String texte1 = data['texte1'];
            String texte2 = data['texte2'];

            // Après la conversion en JSON
            print('Texte 1 récupéré : $texte1');
            print('Texte 2 récupéré : $texte2');
            print("Récupération des textes renvoyés par le serveur");
            //pop up pour demander si je veux garder la photo ou refaire une photo
            showDialog(
              context: context,
              builder: (context) => AlertDialog(
                title: Text(
                  'Photo capturée',
                  style: kTempTextStyle,
                ),
                content: Image.file(File(imagePath)),
                actions: [
                  TextButton(
                    child: Text(
                      'Refaire une photo',
                      style: kTempMiniTextStyle,
                    ),
                    onPressed: () {
                      Navigator.pop(
                        context,
                      );
                    },
                  ),
                  TextButton(
                    child: Text(
                      'OK',
                      style: kTempMiniTextStyle,
                    ),
                    onPressed: () {
                      Navigator.push(
                        context,
                        MaterialPageRoute(
                          builder: (context) {
                            return Screen1(
                              pathImage: imagePath,
                              emotions: texte1,
                              histoire: texte2,
                            );
                          },
                        ),
                      );
                    },
                  ),
                ],
              ),
            );
          } catch (e) {
            print('Erreur: $e');
          }
        },
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.centerFloat,
    );
  }
}
