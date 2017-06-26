# include "defs.h"

static HANDLE processHeap;

//retrieve a handle to the default heap of this process
void Defs::init(void) {
    processHeap = GetProcessHeap();
}

//HeapAlloc wrapper
//allocate a block of memory from a heap
void *Defs::AllocBlock(SIZE_T size) {
    if (processHeap == NULL || size <= 0) return NULL;

    return HeapAlloc(processHeap, HEAP_ZERO_MEMORY, size);
}

//HeapReAlloc wrapper
void *Defs::ReAllocBlock(void *mem, SIZE_T size) {
    if (processHeap == NULL || mem == NULL || size <= 0) return NULL;

    return HeapReAlloc(processHeap, HEAP_ZERO_MEMORY, mem, size);
}

//free a memory block allocated from a heap by the HeapAlloc
void Defs::FreeBlock(void *mem) {
    if (processHeap == NULL || mem == NULL) return;

    HeapFree(processHeap, 0, mem);
    mem = NULL;
}

void Defs::ConsoleLog(QString log) {
    if (log == NULL) return;

    if (1) {
        qDebug() << log;
    }
}
static BOOL is_FileExists(const char *filename) {
    if (filename == NULL)return FALSE;

    return PathFileExistsA(filename);
}

QString Defs::GetFileVersion(QString fName) {
    if (fName == NULL || !is_FileExists(fName.toStdString().c_str())) return "";

    DWORD handle;
    DWORD size = 0;
    void *buffer = 0;
    VS_FIXEDFILEINFO *lpBuffer = 0;
    unsigned int len = 0;
    QString fVersion = 0;

    if ((size = GetFileVersionInfoSizeA(fName.toStdString().c_str(), &handle)) == FALSE) {
        return "";
    }

    if ((buffer = Defs::AllocBlock(size)) == NULL) {
        return "";
    }

    if (GetFileVersionInfoA(fName.toStdString().c_str(), handle, size, buffer) == FALSE) {
        Defs::FreeBlock(buffer);
        return "";
    }

    if (VerQueryValue(buffer, QString("\\").toStdWString().c_str(), (void **)&lpBuffer, &len) == FALSE) {
        Defs::FreeBlock(buffer);
        return "";
    }

    fVersion = QString("%1.%2.%3.%4")
               .arg(HIWORD(lpBuffer->dwFileVersionMS))
               .arg(LOWORD(lpBuffer->dwFileVersionMS))
               .arg(HIWORD(lpBuffer->dwFileVersionLS))
               .arg(LOWORD(lpBuffer->dwFileVersionLS));

   Defs::FreeBlock(buffer);

    return fVersion;
}

unsigned long Defs::GetDriverFileSize(QString fName) {
    if (fName == NULL || !is_FileExists(fName.toStdString().c_str())) return 0;

    HANDLE hFile;
    unsigned long size = 0;
    unsigned long sizeHigh = 0;

    if ((hFile = CreateFileA(fName.toStdString().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if ((size = GetFileSize(hFile,  NULL)) == INVALID_FILE_SIZE) {
        if (GetLastError() == NO_ERROR) {
            if ((size = GetFileSize(hFile,  &sizeHigh)) != INVALID_FILE_SIZE) {
                CloseHandle(hFile);
                return sizeHigh;
            }
        }

        CloseHandle(hFile);
        return 0;
    }

    CloseHandle(hFile);

    return size;
}

QString Defs::GetFileLastWriteTime(QString fName) {
    if (fName == NULL || !is_FileExists(fName.toStdString().c_str())) return 0;

    HANDLE hFile;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, strc_local;
    QString day = "";
    QString month = "";

    if ((hFile = CreateFileA(fName.toStdString().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
        return "";
    }

    if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite) == 0) {
        CloseHandle(hFile);
        return "";
    }

    if (FileTimeToSystemTime(&ftWrite, &stUTC) == 0) {
        CloseHandle(hFile);
        return "";
    }

    if (SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &strc_local) == 0) {
        CloseHandle(hFile);
        return "";
    }

    switch (strc_local.wDayOfWeek) {
        case 0:
            day = "Sunday";
            break;
        case 1:
            day = "Monday";
            break;
        case 2:
            day = "Tuesday";
            break;
        case 3:
            day = "Wednesday";
            break;
        case 4:
            day = "Thursday";
            break;
        case 5:
            day = "Friday";
            break;
        case 6:
            day = "Saturday";
            break;
    };

    switch (strc_local.wMonth) {
        case 1:
            month = "January";
            break;
        case 2:
            month = "February";
            break;
        case 3:
            month = "March";
            break;
        case 4:
            month = "April";
            break;
        case 5:
            month = "May";
            break;
        case 6:
            month = "June";
            break;
        case 7:
            month = "July";
            break;
        case 8:
            month = "August";
            break;
        case 9:
            month = "September";
            break;
        case 10:
            month = "October";
            break;
        case 11:
            month = "November";
            break;
        case 12:
            month = "December";
            break;
    };

    CloseHandle(hFile);

    return QString("%1, %2 %3, %4 %5:%6:%7")
           .arg(day)
           .arg(month)
           .arg(strc_local.wDay)
           .arg(strc_local.wYear)
           .arg(strc_local.wHour)
           .arg(strc_local.wMinute)
           .arg(strc_local.wSecond);
}
