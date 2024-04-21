# Write qt with cmake

```bash
sudo apt-get install libgl1-mesa-dev
sudo apt-get install qtbase5-dev qtchooser  qtbase5-dev-tools qtbase6-dev
sudo apt-get install qt5* qt6*
```

Then import qt library in CMakeLists.txt 
```cmake
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
target_link_libraries(test Qt${QT_VERSION_MAJOR}::Core)
```

For other modules, see [qt5modules-doc](https://doc.qt.io/qt-5/qtmodules.html) or [qt6modules-doc](https://doc.qt.io/qt-6/qtmodules.html)

If you want to use qml, `apt list -a |grep qt |grep qml` may help

Here I cmd `unset GTK_PATH` to avoid a launch failure, I don't know what it will affect, but my desktop-gui works well still, maybe qt has some conflits with gtk.