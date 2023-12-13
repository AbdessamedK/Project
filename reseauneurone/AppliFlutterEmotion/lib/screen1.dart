import 'package:flutter/material.dart';
import 'package:navigation/components/button.dart';
import 'package:navigation/constant.dart';

class Screen1 extends StatefulWidget {
  @override
  Screen1(
      {required this.emotions,
      required this.histoire,
      required this.pathImage});

  String pathImage;
  String emotions;
  String histoire;
  State<Screen1> createState() => _Screen1State();
}

class _Screen1State extends State<Screen1> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white,
        surfaceTintColor: Colors.transparent,
        toolbarOpacity: 1.0,
        centerTitle: true,
        elevation: 0,
        title: Text(
          'RACONTE.AI',
          style: kTempTextStyle,
        ),
      ),
      backgroundColor: kEcranColor,
      body: SingleChildScrollView(
        child: Center(
          child: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Image.asset('asset/${widget.emotions}.png'),
                SizedBox(
                  height: 12.0,
                ),
                Text(
                  widget.emotions,
                  style: kTempTextStyle,
                  textAlign: TextAlign.center,
                ),
                SizedBox(
                  height: 32.0,
                ),
                Text(
                  widget.histoire,
                  style: kTempMiniTextStyle,
                ),
                SizedBox(
                  height: 12.0,
                ),
                BottomButton(
                  label: "Accueil",
                  route: () {
                    //Navigate to Screen 1
                    Navigator.pushNamed(context, '/');
                  },
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
