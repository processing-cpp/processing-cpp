package processing.mode.cpp;

/** For the shared Export Application dialog: AppImages are packed on Linux (CppExport.mkAppImage). */
final class AppImage {
  private AppImage() {}

  static boolean possible() {
    return System.getProperty("os.name", "").toLowerCase().contains("linux");
  }
}
