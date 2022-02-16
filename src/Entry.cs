using System;
using System.Windows;
using System.Runtime.InteropServices;

namespace GeodeApp {
    public partial class App : Application {
        [DllImport("kernel32.dll")]
        static extern bool AttachConsole(int dwProcessId);

        [STAThread]
        public static void Main(String[] args) {
            AttachConsole(-1);
            App app = new App();
            app.Run(new MainWindow());
        }
    }
}
