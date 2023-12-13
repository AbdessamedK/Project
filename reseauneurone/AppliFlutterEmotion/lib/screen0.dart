import 'package:flutter/material.dart';
import 'package:navigation/components/button.dart';
import 'package:navigation/constant.dart';

class Screen0 extends StatelessWidget {
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
      body: Center(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            BottomButton(
              label: "Prend une photo",
              route: () {
                //Navigate to Screen 1
                Navigator.pushNamed(context, '/camera');
              },
            ),
          ],
        ),
      ),
    );
  }
}
//pour aller de route en route
//            {
//                 //Navigate to Screen 1
//                 Navigator.pushNamed(context, '/first');
//               },