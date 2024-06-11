import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:shared_preferences/shared_preferences.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await initializeNotifications();
  runApp(const MyApp());
}

FlutterLocalNotificationsPlugin flutterLocalNotificationsPlugin = FlutterLocalNotificationsPlugin();

Future<void> initializeNotifications() async {
  const AndroidInitializationSettings initializationSettingsAndroid =
      AndroidInitializationSettings('@mipmap/ic_launcher');

  const DarwinInitializationSettings initializationSettingsIOS = DarwinInitializationSettings();

  const InitializationSettings initializationSettings = InitializationSettings(
    android: initializationSettingsAndroid,
    iOS: initializationSettingsIOS,
  );

  await flutterLocalNotificationsPlugin.initialize(
    initializationSettings,
    onDidReceiveNotificationResponse: (NotificationResponse notificationResponse) async {
      if (notificationResponse.payload != null) {
        debugPrint('notification payload: ${notificationResponse.payload}');
      }
    },
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: AlarmPage(),
    );
  }
}

class AlarmPage extends StatefulWidget {
  const AlarmPage({Key? key});

  @override
  _AlarmPageState createState() => _AlarmPageState();
}

class _AlarmPageState extends State<AlarmPage> {
  BluetoothConnection? connection;
  bool isConnected = false;

  FlutterLocalNotificationsPlugin flutterLocalNotificationsPlugin =
      FlutterLocalNotificationsPlugin();

  late SharedPreferences _prefs;

  List<TextEditingController> medicationControllers = List.generate(
    6,
    (index) => TextEditingController(),
  );

  List<TextEditingController> intervalControllers = List.generate(
    6,
    (index) => TextEditingController(),
  );

  List<bool> alarmActive = List.generate(6, (index) => false);

  final int maxReminders = 6;

  List<Timer?> timers = List.generate(6, (index) => null);

  @override
  void initState() {
    super.initState();
    initializeNotifications();
    loadSavedSettings();
  }

  Future<void> initializeNotifications() async {
    const AndroidInitializationSettings initializationSettingsAndroid =
        AndroidInitializationSettings('@mipmap/ic_launcher');

    const InitializationSettings initializationSettings = InitializationSettings(
      android: initializationSettingsAndroid,
    );

    await flutterLocalNotificationsPlugin.initialize(
      initializationSettings,
      onDidReceiveNotificationResponse: (NotificationResponse notificationResponse) async {
        if (notificationResponse.payload != null) {
          debugPrint('notification payload: ${notificationResponse.payload}');
        }
      },
    );
  }

  Future<void> loadSavedSettings() async {
    _prefs = await SharedPreferences.getInstance();
    setState(() {
      for (int i = 0; i < maxReminders; i++) {
        medicationControllers[i].text = _prefs.getString('medication_$i') ?? '';
        intervalControllers[i].text = _prefs.getString('interval_$i') ?? '';
        alarmActive[i] = _prefs.getBool('alarmActive_$i') ?? false;
        if (alarmActive[i]) {
          setReminder(i);
        }
      }
    });
  }

  Future<void> saveSettings() async {
    for (int i = 0; i < maxReminders; i++) {
      _prefs.setString('medication_$i', medicationControllers[i].text);
      _prefs.setString('interval_$i', intervalControllers[i].text);
      _prefs.setBool('alarmActive_$i', alarmActive[i]);
    }
    configureReminders();
    ScaffoldMessenger.of(context).showSnackBar(
      const SnackBar(content: Text('Configuraciones guardadas.')),
    );
  }

  Future<void> _sendNotificationAndSignal(int index) async {
    if (connection != null && isConnected) {
      // Enviar señal Bluetooth
      connection!.output.add(Uint8List.fromList([index + 1]));
      await connection!.output.allSent;

      // Enviar notificación
      String title = medicationControllers[index].text;

      const AndroidNotificationDetails androidPlatformChannelSpecifics =
          AndroidNotificationDetails(
        'your_channel_id',
        'your_channel_name',
        channelDescription: 'your_channel_description',
        importance: Importance.max,
        priority: Priority.high,
        sound: RawResourceAndroidNotificationSound('notification_sound'),
      );
      const NotificationDetails platformChannelSpecifics = NotificationDetails(
        android: androidPlatformChannelSpecifics,
      );

      await flutterLocalNotificationsPlugin.show(
        index,
        'Medicamento ${index + 1}',
        'Es hora de tomar tu $title',
        platformChannelSpecifics,
        payload: 'item x',
      );
    } else {
      print('No hay conexión Bluetooth');
    }
  }

  Future<void> _scheduleNotification(int index) async {
    int interval = int.tryParse(intervalControllers[index].text) ?? 0;
    if (interval <= 0) return;

    timers[index]?.cancel(); // Cancel any existing timer
    timers[index] = Timer.periodic(Duration(minutes: interval), (timer) async {
      await _sendNotificationAndSignal(index);
    });
  }

  Future<void> setReminder(int index) async {
    if (intervalControllers[index].text.isNotEmpty) {
      _scheduleNotification(index);
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Recordatorio para el medicamento ${index + 1} establecido.')),
      );
    }
  }

  void configureReminders() {
    for (int i = 0; i < maxReminders; i++) {
      if (alarmActive[i]) {
        setReminder(i);
      } else {
        timers[i]?.cancel();
      }
    }
  }

  Future<void> _connectToDevice() async {
    FlutterBluetoothSerial bluetoothSerial = FlutterBluetoothSerial.instance;
    List<BluetoothDevice> devices = await bluetoothSerial.getBondedDevices();

    BluetoothDevice? selectedDevice = await showDialog<BluetoothDevice>(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Selecciona un dispositivo'),
          content: SingleChildScrollView(
            child: Column(
              children: devices.map((device) {
                return ListTile(
                  title: Text(
                    device.name ?? 'Dispositivo sin nombre',
                    style: const TextStyle(color: Color.fromRGBO(26, 26, 46, 1)),
                  ),
                  onTap: () => Navigator.of(context).pop(device),
                );
              }).toList(),
            ),
          ),
        );
      },
    );

    if (selectedDevice != null) {
      try {
        connection = await BluetoothConnection.toAddress(selectedDevice.address);
        setState(() {
          isConnected = true;
        });
        print('Conectado a ${selectedDevice.name}');
      } catch (e) {
        print('Error al conectar: $e');
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(content: Text('Error al conectar: $e')),
        );
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Alarm Page'),
        actions: [
          IconButton(
            onPressed: saveSettings,
            icon: const Icon(Icons.save),
          ),
        ],
      ),
      body: ListView.builder(
        itemCount: maxReminders,
        itemBuilder: (context, index) {
          return ListTile(
            title: Row(
              children: [
                Expanded(
                  child: Text(
                    'Medicamento ${index + 1}',
                    textAlign: TextAlign.left,
                  ),
                ),
                Switch(
                  value: alarmActive[index],
                  onChanged: (value) {
                    setState(() {
                      alarmActive[index] = value;
                    });
                  },
                ),
              ],
            ),
            subtitle: Column(
              children: [
                TextField(
                  controller: intervalControllers[index],
                  decoration: const InputDecoration(labelText: 'Intervalo de tiempo (minutos)'),
                  keyboardType: TextInputType.number,
                  enabled: alarmActive[index],
                ),
                ElevatedButton(
                  onPressed: () => _sendNotificationAndSignal(index),
                  child: Text('Dispensar ${medicationControllers[index].text}'),
                ),
              ],
            ),
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _connectToDevice,
        child: const Icon(Icons.bluetooth),
      ),
    );
  }
}
