import 'package:flutter/material.dart';
import 'package:navigation/screen0.dart';
import 'package:navigation/screen1.dart';
import 'package:navigation/screenPicture.dart';
import 'package:navigation/screen0.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      initialRoute: '/',
      routes: {
        '/': (context) => Screen0(),
        '/camera': (context) => CameraScreen(),
      },
      //theme: ThemeData.dark(),
    );
  }
}
