RUS



ПРОЕКТ

    Данный проект собирает библиотеку, которая перенаправляет запрсы файловой
    системы на чтение и запись. Библиотеку можно использовать как и в режиме
    ядра, так и в режиме пользователя.

    Различие режимов работы библиотеки заключается в способе выдедения и
    освобождения памяти.

    Также производится сборка утилиты, которая тестирует библиотеку в режиме
    пользователя.



СБОРКА

    Сборка проекта осуществлялась с помощью Windows Driver Kit Version 7.1.0
    https://www.microsoft.com/en-us/download/details.aspx?id=11800 с
    параметрами "chk x64 WIN7" (x64 Checked Build Environment.bat) на
    операционной системе Windows 7 x64 Professional SP1.



ТИПЫ ДАННЫХ

    LIB_MODE - режим библиотеки (значения KERNEL_MODE, USER_MODE)

    LIB_BLOCK - номер блока физических и виртуальных носителей

    LIB_TABLE, *LIB_PTABLE - RTL Generic AVL Table (AVL-дерево, реализованное
    в библиотеке Windows ntoskrnl.lib)

    LIB_BITMASK - битовая маска

    LIB_NODE, *LIB_PNODE - узел AVL-дерева

    LIB_PTABLE_ARRAY, LIB_PNODE_ARRAY, LIB_NODE_ARRAY - массивы с динамической памятью



СТРУКТУРА БИБЛИОТЕКИ

    VOID InitLibrary
    (
        BOOLEAN _LibraryMode,
        ULONG _LogicalFileSize,
        ULONG _PhysicalFileSize,
        int _MaxSnapshotCount
    ) - инициализировать библиотеку
    
    VOID DeinitLibrary() - деинициализировать библиотеку
    
    LIB_BLOCK CheckBlock(LIB_BLOCK A, BOOLEAN *ok) - проверить отображение
    одного блока
    
    LIB_PNODE_ARRAY CheckInterval(LIB_PTABLE Table, LIB_BLOCK A, LIB_BLOCK k) -
    проверить отображение интервала (подряд идущих блоков)
    

    int SnapshotCount()     - количество записаных снапшотов
    int SnapshotCapacity()  - максимальное количество снапшотов
    int SnapshotMake()      - создать снапшот, записать в первый свободный слот
    
    BOOLEAN SnapshotSave(int n)     - создать снапшот, записать в указанный слот
    BOOLEAN SnapshotLoad(int n)     - загрузить указанный снапшот
    BOOLEAN SnapshotDelete(int n)   - удалить указанный снапшот

    BOOLEAN PrepareToWrite(LIB_BLOCK A, LIB_BLOCK k) - подготовить интервал
    к записи (произвести перенаправление записи на свободные блоки)
    
