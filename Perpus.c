#include <stdio.h>
#include <string.h>
struct User
{
  int id;
  char username[50];
  char password[50];
  int role;
};

struct Book
{
  int id;
  char title[50];
  char author[50];
  int isAvailable;
};

struct UserBook
{
  int userId;
  int idBook;
};

void itemBook(int i, struct Book book, int showAvailable)
{
  printf(" %3d   | ID: %d \n", i + 1, book.id);
  printf("       | JUDUL: %s \n", book.title);
  printf("       | AUTHOR: %s \n", book.author);
  if (showAvailable)
  {
    printf("       | TERSEDIA: %d \n", book.isAvailable);
  }
  printf("-------------------------------------------\n");
}

void showListBooks(struct Book listBook[], int totalBooks)
{
    printf("-------------------------------------------\n");
    printf("| No | ID  | JUDUL                        | AUTHOR                     | TERSEDIA |\n");
    printf("-------------------------------------------\n");
    
    if (totalBooks == 0)
    {
        printf("|    |     | BELUM ADA BUKU YANG DITAMBAHKAN...\n");
        printf("-------------------------------------------\n");
        return;
    }

    for (int i = 0; i < totalBooks; i++)
    {
        printf("| %3d | %3d | %-28s | %-24s | %d       |\n", 
               i + 1, listBook[i].id, listBook[i].title, listBook[i].author, listBook[i].isAvailable);
    }
    printf("-------------------------------------------\n");
}

void addBook(struct Book listBooks[], int *totalBooks)
{
  char inp;
  struct Book newBook;
  do
  {
    printf("Masukan judul buku: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    int titleLength = strlen(newBook.title) - 1;
    newBook.title[titleLength] = '\0';

    printf("Masukan author buku: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    int authorLength = strlen(newBook.author) - 1;
    newBook.author[authorLength] = '\0';

    printf("Stok buku tersedia: ");
    scanf("%d", &newBook.isAvailable);
    getchar();

    newBook.id = listBooks[*totalBooks - 1].id + 1;
    listBooks[*totalBooks] = newBook;

    printf("BUKU BERHASIL DITAMBAH...\n");
    *totalBooks += 1;

    printf("Tambah buku lagi? (y/n): ");
    scanf("%c", &inp);
    getchar();
  } while (inp == 'y' || inp == 'Y');
}

void editBook(struct Book listBooks[], int totalBooks)
{
  int id;
  int isFind = 0;
  printf("ID buku yang ingin di edit: ");
  scanf("%d", &id);
  getchar();

  for (int i = 0; i < totalBooks; i++)
  {
    if (id == listBooks[i].id)
    {
      isFind = 1;
      printf("Nama buku: ");
      fgets(listBooks[i].title, sizeof(listBooks[i].title), stdin);
      int titleLength = strlen(listBooks[i].title) - 1;
      listBooks[i].title[titleLength] = '\0';

      printf("Author: ");
      fgets(listBooks[i].author, sizeof(listBooks[i].author), stdin);
      int authorLength = strlen(listBooks[i].author) - 1;
      listBooks[i].author[authorLength] = '\0';

      printf("Stok: ");
      scanf("%d", &listBooks[i].isAvailable);
      getchar();

      printf("EDIT BUKU BERHASIL!!!\n");
      return;
    }
  }

  if (!isFind)
  {
    printf("ID BUKU TIDAK TERSEDIA!!!\n");
  }
}

void deleteBook(struct Book listBooks[], int *totalBooks, struct UserBook listBorrowed[], int *totalBorrowed)
{
  int id;
  int deleted = 0;
  int deletedId = 0;
  int deletedInBorrow = 0;

  printf("ID buku yang ingin dihapus: ");
  scanf("%d", &id);
  getchar();

  for (int i = 0; i < *totalBooks; i++)
  {
    if (listBooks[i].id == id)
    {
      deleted = 1;
      deletedId = listBooks[i].id;
    }
    if (deleted && i < *totalBooks - 1)
    {
      listBooks[i] = listBooks[i + 1];
    }
  }
  if (deleted)
  {
    printf("HAPUS BUKU BERHASIL...\n");
    for (int i = 0; i < *totalBorrowed; i++)
    {
      if (listBorrowed[i].idBook == deletedId)
      {
        deletedInBorrow++;
      }

      if (deletedInBorrow && i < *totalBorrowed - 1)
      {
        listBorrowed[i] = listBorrowed[i + 1];
      }
    }

    *totalBorrowed -= deletedInBorrow;
    *totalBooks -= 1;
  }
  else
  {
    printf("BUKU TIDAK DITEMUKAN!!!\n");
  }
}

void manageBook(struct Book listBooks[], int *totalBooks, struct UserBook listBorrowed[], int *totalBorrowed)
{
  int selected;

  do
  {
    printf("MENU TERSEDIA: \n");
    printf("1. LIST BUKU\n");
    printf("2. TAMBAH BUKU\n");
    printf("3. EDIT BUKU\n");
    printf("4. HAPUS BUKU\n");
    printf("5. KEMBALI\n");

    printf("Pilih menu: ");
    scanf("%d", &selected);
    getchar();

    switch (selected)
    {
    case 1:
      showListBooks(listBooks, *totalBooks);
      break;
    case 2:
      addBook(listBooks, totalBooks);
      break;
    case 3:
      editBook(listBooks, *totalBooks);
      break;
    case 4:
      deleteBook(listBooks, totalBooks, listBorrowed, totalBorrowed);
      break;
    case 5:
      return;
      break;

    default:
      printf("PILIHAN TIDAK VALID!!!\n");
      break;
    }
  } while (selected != 5);
}

void borrowBook(struct Book listBooks[], int totalBooks, struct UserBook listBorrowed[], int *totalBorrowed, int userId)
{
  int selected;
  int isFind = 0;
  int idBook = -1;
  int count = 0;

  printf("BUKU TERSEDIA:\n");
  printf("-------------------------------------------\n");

  for (int i = 0; i < totalBooks; i++)
  {
    if (listBooks[i].isAvailable)
    {
      itemBook(count, listBooks[i], 0);
      count++;
    }
  }
  if (!totalBooks || !count)
  {
    printf("TIDAK ADA BUKU YANG TERSEDIA...\n");
    printf("-------------------------------------------\n");
    return;
  }
  printf("MASUKAN ID BUKU YANG INGIN DIPINJAM: ");
  scanf("%d", &selected);
  getchar();

  for (int i = 0; i < *totalBorrowed; i++)
  {
    if (listBorrowed[i].userId == userId && listBorrowed[i].idBook == selected)
    {
      printf("ANDA TELAH MEMINJAM BUKU INI!!!\n");
      return;
    }
  }

  for (int i = 0; i < totalBooks; i++)
  {
    if (selected == listBooks[i].id && listBooks[i].isAvailable > 0)
    {
      isFind = 1;
      listBooks[i].isAvailable -= 1;
      listBorrowed[*totalBorrowed].userId = userId;
      listBorrowed[*totalBorrowed].idBook = selected;
      *totalBorrowed += 1;
      printf("BUKU BERHASIL DIPINJAM...\n");
      break;
    }
  }
  if (!isFind)
  {
    printf("BUKU TIDAK TERSEDIA!!!\n");
  }
}

struct Book getBookById(int idBook, struct Book listBooks[], int totalBooks)
{
  for (int i = 0; i < totalBooks; i++)
  {
    if (idBook == listBooks[i].id)
    {
      return listBooks[i];
    }
  }
}

void showListBorrowed(struct UserBook *listBorrowed, int totalBorrowed, int userId, struct Book *listBooks, int totalBooks)
{
  int hasBorrowed = 0;

  printf("BUKU YANG ANDA PINJAM: \n");
  printf("-------------------------------------------\n");
  for (int i = 0; i < totalBorrowed; i++)
  {
    if (userId == listBorrowed[i].userId)
    {
      struct Book book = getBookById(listBorrowed[i].idBook, listBooks, totalBooks);
      itemBook(hasBorrowed, book, 0);
      hasBorrowed++;
    }
  }
  if (!hasBorrowed)
  {
    printf("ANDA BELUM MEMINJAM BUKU!!!\n");
    printf("-------------------------------------------\n");
  }
}

void returnBook(int userId, struct UserBook listBorrowed[], int *totalBorrrowed, struct Book listBooks[], int totalBooks)
{
  int returned = 0;
  int id;

  printf("MASUKAN ID BUKU YANG INGIN DIKEMBALIKAN: ");
  scanf("%d", &id);
  getchar();

  for (int i = 0; i < *totalBorrrowed; i++)
  {
    if (listBorrowed[i].idBook == id && listBorrowed[i].userId == userId)
    {
      returned = 1;
    }
    if (returned == 1 && i < *totalBorrrowed - 1)
    {
      listBorrowed[i] = listBorrowed[i + 1];
    }
  }
  if (returned)
  {
    for (int i = 0; i < totalBooks; i++)
    {
      if (listBooks[i].id == id)
      {
        listBooks[i].isAvailable += 1;
        *totalBorrrowed -= 1;
        break;
      }
    }
    printf("BUKU BERHASIL DIKEMBALIKAN...\n");
    return;
  }
  printf("ANDA TIDAK MEMINJAM BUKU DENGAN ID TERSEBUT!!!\n");
}

void displayUser(int userId, struct Book listBooks[], int totalBooks, struct UserBook listBorrowed[], int *totalBorrowed)
{
  int selected;
  printf("SELAMAT DATANG BRO \n");

  do
  {
    printf("MENU TERSEDIA: \n");
    printf("1. PINJAM BUKU\n");
    printf("2. CEK BUKU YANG DIPINJAM\n");
    printf("3. KEMBALIKAN BUKU\n");
    printf("4. LOGOUT\n");

    printf("PILIH MENU: ");
    scanf("%d", &selected);
    getchar();

    switch (selected)
    {
    case 1:
      borrowBook(listBooks, totalBooks, listBorrowed, totalBorrowed, userId);
      break;
    case 2:
      showListBorrowed(listBorrowed, *totalBorrowed, userId, listBooks, totalBooks);
      break;
    case 3:
      returnBook(userId, listBorrowed, totalBorrowed, listBooks, totalBooks);
      break;
    case 4:
      printf("Logout berhasil...\n");
      break;
    default:
      printf("PILIHAN TIDAK TERSEDIA!!!\n");
      break;
    }
  } while (selected != 4);
}

void displayAdmin(int userId, struct Book listBook[], int *totalBooks, struct UserBook listBorrowed[], int *totalBorrowed)
{
  int selected;
  printf("SELAMAT DATANG ADMIN: \n");

  do
  {
    printf("MENU TERSEDIA: \n");
    printf("1. MANAJEMEN BUKU\n");
    printf("2. PINJAM BUKU\n");
    printf("3. CEK BUKU YANG DIPINJAM\n");
    printf("4. KEMBALIKAN BUKU\n");
    printf("5. LOGOUT\n");

    printf("Pilih menu: ");
    scanf("%d", &selected);
    getchar();

    switch (selected)
    {
    case 1:
      manageBook(listBook, totalBooks, listBorrowed, totalBorrowed);
      break;
    case 2:
      borrowBook(listBook, *totalBooks, listBorrowed, totalBorrowed, userId);
      break;
    case 3:
      showListBorrowed(listBorrowed, *totalBorrowed, userId, listBook, *totalBooks);
      break;
    case 4:
      returnBook(userId, listBorrowed, totalBorrowed, listBook, *totalBooks);
      break;
    case 5:
      printf("Logout berhasil...\n");
      break;

    default:
      printf("PILIHAN TIDAK TERSEDIA!!!\n");
      break;
    }
  } while (selected != 5);
}

struct User login()
{
  int findedUser = 0;
  struct User user;
  struct User listUser[] = {
      {1, "admin", "rahasiaAdmin", 1},
      {2, "dominic", "rahasia", 2},
  };
  int totalUsers = sizeof(listUser) / sizeof(listUser[0]);
  char iUser[50], iPass[50];

  printf("username: ");
  scanf("%s", iUser);
  getchar();

  printf("password: ");
  scanf("%s", iPass);
  getchar();

  for (int i = 0; i < totalUsers; i++)
  {
    if (strcmp(listUser[i].username, iUser) == 0)
    {
      findedUser = 1;
      user = listUser[i];
      break;
    }
  }

  if (findedUser)
  {
    int isMatch = strcmp(iPass, user.password);
    if (isMatch != 0)
    {
      user.id = 0;
      user.role = 0;
      printf("PASSWORD SALAH!!!\n");
    }
  }
  else
  {
    user.id = 0;
    user.role = 0;
    printf("USERNAME TIDAK DITEMUKAN!!!\n");
  }
  return user;
}

int main()
{
  struct User user;
  struct Book listBooks[100];
  int totalBooks = 0;
  struct UserBook listBorrowed[100];
  int totalBorrowed = 0;

  printf("LOGIN PERPUSTAKAAN DIGITAL\n");
  while (1)
  {
    user = login();
    switch (user.role)
    {
    case 1:
      displayAdmin(user.id, listBooks, &totalBooks, listBorrowed, &totalBorrowed);
      break;
    case 2:
      displayUser(user.id, listBooks, totalBooks, listBorrowed, &totalBorrowed);
      break;

    default:
      printf("----------------------------------\n");
      printf("LOGIN GAGAL! SILAHKAN COBA LAGI...\n");
      printf("----------------------------------\n");
      break;
    }
  };
  return 0;
}
