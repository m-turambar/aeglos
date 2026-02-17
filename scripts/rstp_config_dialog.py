import tkinter as tk
from tkinter import messagebox
import os
import sys

def save_rstp_config():
    user = entry_user.get().strip()
    password = entry_pass.get().strip()
    ip = entry_ip.get().strip()

    if not user or not password or not ip:
        messagebox.showerror("Error", "Username, password and IP required")
        return

    try:
        os.makedirs(os.path.dirname(output_path), exist_ok=True)
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(user + "\n")
            f.write(password + "\n")
            f.write(ip + "\n")
    except Exception as e:
        messagebox.showerror("Error", str(e))
        return

    root.destroy()

if len(sys.argv) < 2:
    print("Usage: credentials_dialog.py <output_file>")
    sys.exit(1)

output_path = sys.argv[1]

root = tk.Tk()
root.title("RSTP Configuration")
root.geometry("250x200")
root.resizable(False, False)

tk.Label(root, text="Username:").pack(pady=(10,0))
entry_user = tk.Entry(root)
entry_user.pack()

tk.Label(root, text="Password:").pack(pady=(10,0))
entry_pass = tk.Entry(root, show="*")
entry_pass.pack()

tk.Label(root, text="IP:").pack(pady=(10,0))
entry_ip = tk.Entry(root)
entry_ip.pack()

tk.Button(root, text="Save", command=save_rstp_config).pack(pady=10)

root.mainloop()