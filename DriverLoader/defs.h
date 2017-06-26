#ifndef DEFS_H
#define DEFS_H

//include files
#include <windows.h>
# include<TlHelp32.h>
#include <Shlwapi.h>
#include <QMainWindow>
#include <QDialog>
#include <QStringList>
#include <QList>
#include<QMessageBox>
#include <QFileDialog>
#include <QDir>
#include<QFileInfo>
#include<QComboBox>
#include <QStringListModel>
#include <QStringList>
#include<QDebug>
#include<QCloseEvent>



namespace Defs{
    //initialize common lib stuff
    void init(void);

    //Allocates a block of memory from a heap.
    void *AllocBlock(SIZE_T size);

    //Reallocates a block of memory from a heap.
    void *ReAllocBlock(void *mem, SIZE_T size);

    //free a memory block allocated from a heap by the hAlloc
    void FreeBlock(void *mem);

    //console.log
    void ConsoleLog(QString log);
    //get driver file version
    QString GetFileVersion(QString fName);

    //get driver file size in bytes
    unsigned long GetDriverFileSize(QString fName);

    //get driver file last-write time
    QString GetFileLastWriteTime(QString fName);

}


#endif // DEFS_H
