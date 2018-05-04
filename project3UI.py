#!/usr/bin/python3

from tkinter import *
from MLB import *
import sqlite3
import datetime

def loadTask():
    c = db.cursor()
    query = c.execute('SELECT task, time,location FROM schedules')
    load = c.fetchall()
    for i in load:
        mlb.insert(END,i)

def addTask():
    print(taskVar.get())
    c = db.cursor()
    date = c.execute("SELECT date('now')")
    fetch = c.fetchone()
    print(fetch)
    added = (taskVar.get(),fetch[0] + ' ' + timeVar.get(),lVar.get())
    print(added)
    mlb.insert(END, added)
    c = db.cursor()
    insert = c.execute("INSERT INTO SCHEDULES (task, time, location) VALUES(?,?,?)",added)
    db.commit()
    
def removeTask():
    print(mlb.get(mlb.curselection()[0]))
    c = db.cursor()
    delete = c.execute("DELETE FROM SCHEDULES WHERE task = ? AND time = ? AND location = ?"
                       ,mlb.get(mlb.curselection()[0]))
    db.commit()
    mlb.delete(mlb.curselection())
    
'''def setup():
    c = db.cursor()
    c.execute("CREATE TABLE IF NOT EXISTS schedule(id INTEGER PRIMARY KEY AUTOINCREMENT, task TEXT, location TEXT, time TEXT);")
    db.commit()'''

if __name__ == '__main__':
    db = sqlite3.connect('db')
    '''setup()
    print("Done")'''
    top = Tk()
    Label(top, text = "Task").grid(row = 0)
    Label(top, text = "Location").grid(row = 1)
    Label(top, text = "Time").grid(row = 2)

    lVar = StringVar(top)
    lVar.set("Choose a location")
    locationmenu = OptionMenu(top, lVar,
                              "Bathroom", "Kitchen", "Bedroom")
    locationmenu.grid(row = 1, column = 1)

    timeVar = StringVar(top)
    timeVar.set("Choose a time")
    timemenu = OptionMenu(top, timeVar,
                   "00:00:00", "00:30:00", "01:00:00", "01:30:00", "02:00:00", "02:30:00",
                   "03:00:00", "03:30:00", "04:00:00", "04:30:00", "05:00:00", "05:30:00",
                   "06:00:00", "06:30:00", "07:00:00", "07:30:00", "08:00:00", "08:30:00",
                   "09:00:00", "09:30:00", "10:00:00", "10:30:00", "11:00:00", "11:30:00",
                   "12:00:00", "12:30:00", "13:00:00", "13:30:00", "14:00:00", "02:30:00",
                   "15:00:00", "15:30:00", "16:00:00", "16:30:00", "17:00:00", "05:30:00",
                   "18:00:00", "18:30P:00", "19:00:00", "19:30:00", "20:00:00", "08:30:00",
                   "21:00:00", "21:30:00", "22:00:00", "22:30:00", "23:00:00", "23:30:00")
    timemenu.grid(row = 2, column = 1)

    taskVar = StringVar(top)
    taskVar.set("Choose a task")
    taskmenu = OptionMenu(top, taskVar,
                              "wakeUp", "Use the restroom", "Eat breakfast", "takeMeds",
                          "drinkWater", "gotoBed","Eat lunch", "Eat dinner", "Get dressed",
                          "gotoBath", "gotoKitchen")
    taskmenu.grid(row = 0, column = 1)

    AddB = Button(top, text = "Add", command = addTask).grid(row = 3)
    RemoveB = Button(top, text = "Remove", command = removeTask).grid(row = 3, column = 1)
    
    mlb = MultiListbox(top, (('Task', 15),('Time', 20),('Location', 40)))
    mlb.place(x = 0, y = 120, width = 400)
    loadTask()
    
    top.geometry("400x400+10+10")
    top.title('Home Scheduler')
    top.mainloop()
