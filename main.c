#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRANS 300
#define PASSWORD "fintrix123"

struct Transaction {
    char type[10];       // income / expense
    char category[30];   // kategori baru
    char note[100];      // deskripsi bebas (pakai spasi)
    float amount;
    char date[30];
};

struct Transaction transactions[MAX_TRANS];
int count = 0;

// ======== Deklarasi fungsi ========
void login();
void addTransaction();
void showTransactions();
void saveToFile();
void loadFromFile();
void deleteTransaction();
void filterTransactions();
void exportCSV();
void showChart();
void listCategories();
void clearBuffer();

// ======== Fungsi login ========
void login() {
    char input[50];
    int tries = 3;

    while (tries > 0) {
        printf("🔐 Masukkan password Fintrix: ");
        scanf("%s", input);

        if (strcmp(input, PASSWORD) == 0) {
            printf("✅ Akses diterima. Selamat datang!\n");
            return;
        } else {
            tries--;
            printf("❌ Password salah. Sisa percobaan: %d\n", tries);
        }
    }

    printf("🚫 Akses ditolak. Program berhenti.\n");
    exit(0);
}

// ======== Tambah transaksi ========
void addTransaction() {
    if (count >= MAX_TRANS) {
        printf("🚫 Kapasitas data penuh!\n");
        return;
    }

    struct Transaction t;
    time_t now = time(NULL);
    strcpy(t.date, ctime(&now));
    t.date[strlen(t.date) - 1] = '\0';

    printf("\nPilih tipe transaksi (income/expense): ");
    scanf("%s", t.type);
    clearBuffer();

    printf("Masukkan nominal (Rp): ");
    scanf("%f", &t.amount);
    clearBuffer();

    listCategories();
    printf("Pilih kategori (ketik nama kategori): ");
    fgets(t.category, sizeof(t.category), stdin);
    t.category[strcspn(t.category, "\n")] = '\0'; // hapus newline

    printf("Keterangan (boleh pakai spasi): ");
    fgets(t.note, sizeof(t.note), stdin);
    t.note[strcspn(t.note, "\n")] = '\0';

    transactions[count++] = t;
    saveToFile();
    printf("✅ Transaksi berhasil disimpan!\n");
}

// ======== List kategori ========
void listCategories() {
    printf("\nKategori yang tersedia:\n");
    printf("1️⃣  Food & Drink\n");
    printf("2️⃣  Transport\n");
    printf("3️⃣  Shopping\n");
    printf("4️⃣  Investment\n");
    printf("5️⃣  Entertainment\n");
    printf("6️⃣  Others\n\n");
}

// ======== Tampilkan semua transaksi ========
void showTransactions() {
    printf("\n========== 💸 Riwayat Transaksi ==========\n");
    float totalIncome = 0, totalExpense = 0;

    for (int i = 0; i < count; i++) {
        printf("%d. [%s] Rp%.2f | %s | %s | %s\n",
               i + 1,
               transactions[i].type,
               transactions[i].amount,
               transactions[i].category,
               transactions[i].note,
               transactions[i].date);

        if (strcmp(transactions[i].type, "income") == 0)
            totalIncome += transactions[i].amount;
        else
            totalExpense += transactions[i].amount;
    }

    printf("-------------------------------------------\n");
    printf("📈 Total Income : Rp%.2f\n", totalIncome);
    printf("📉 Total Expense: Rp%.2f\n", totalExpense);
    printf("💰 Net Balance  : Rp%.2f\n", totalIncome - totalExpense);
    printf("===========================================\n");
}

// ======== Simpan ke file ========
void saveToFile() {
    FILE *fp = fopen("fintrix.txt", "w");
    if (!fp) {
        printf("❌ Gagal menyimpan file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %f %s|%s|%s\n",
                transactions[i].type,
                transactions[i].amount,
                transactions[i].category,
                transactions[i].note,
                transactions[i].date);
    }

    fclose(fp);
}

// ======== Load file ========
void loadFromFile() {
    FILE *fp = fopen("fintrix.txt", "r");
    if (!fp) return;

    while (fscanf(fp, "%s %f %[^|]|%[^|]|%[^\n]",
                  transactions[count].type,
                  &transactions[count].amount,
                  transactions[count].category,
                  transactions[count].note,
                  transactions[count].date) == 5) {
        count++;
    }

    fclose(fp);
}

// ======== Hapus transaksi ========
void deleteTransaction() {
    int index;
    printf("Masukkan nomor transaksi yang ingin dihapus: ");
    scanf("%d", &index);
    index--;

    if (index < 0 || index >= count) {
        printf("❌ Nomor tidak valid!\n");
        return;
    }

    for (int i = index; i < count - 1; i++) {
        transactions[i] = transactions[i + 1];
    }

    count--;
    saveToFile();
    printf("🗑️  Transaksi berhasil dihapus!\n");
}

// ======== Filter berdasarkan kategori ========
void filterTransactions() {
    char category[30];
    clearBuffer();
    printf("Masukkan kategori untuk difilter: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = '\0';

    printf("\n========== 🔍 Filter: %s ==========\n", category);
    for (int i = 0; i < count; i++) {
        if (strcasecmp(transactions[i].category, category) == 0) {
            printf("%d. [%s] Rp%.2f | %s | %s\n",
                   i + 1,
                   transactions[i].type,
                   transactions[i].amount,
                   transactions[i].category,
                   transactions[i].note);
        }
    }
    printf("=====================================\n");
}

// ======== Export ke CSV (Excel) ========
void exportCSV() {
    FILE *fp = fopen("fintrix.csv", "w");
    if (!fp) {
        printf("❌ Gagal membuat file CSV.\n");
        return;
    }

    fprintf(fp, "Type,Amount,Category,Note,Date\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%.2f,%s,%s,%s\n",
                transactions[i].type,
                transactions[i].amount,
                transactions[i].category,
                transactions[i].note,
                transactions[i].date);
    }

    fclose(fp);
    printf("✅ Data berhasil diexport ke 'fintrix.csv'\n");
}

// ======== Grafik sederhana ========
void showChart() {
    float income = 0, expense = 0;

    // Hitung total income & expense
    for (int i = 0; i < count; i++) {
        if (strcmp(transactions[i].type, "income") == 0)
            income += transactions[i].amount;
        else
            expense += transactions[i].amount;
    }

    // Skala visual bar chart (1 simbol = Rp10.000)
    int incomeBars = (int)(income / 10000);
    int expenseBars = (int)(expense / 10000);

    printf("\n========== 📊 Income vs Expense ==========\n");

    // Warna hijau untuk income, merah untuk expense (kalau terminal support ANSI)
    printf("\033[1;32m"); // Hijau terang
    printf("Income : ");
    for (int i = 0; i < incomeBars && i < 50; i++) printf("#");
    printf(" (Rp%.2f)\n", income);
    printf("\033[0m"); // Reset warna

    printf("\033[1;31m"); // Merah terang
    printf("Expense: ");
    for (int i = 0; i < expenseBars && i < 50; i++) printf("-");
    printf(" (Rp%.2f)\n", expense);
    printf("\033[0m"); // Reset warna

    // Legend penjelasan skala
    printf("\n------------------------------------------\n");
    printf("📘 Legend:\n");
    printf("  # = Rp10.000 Income\n");
    printf("  - = Rp10.000 Expense\n");
    printf("  Maksimal bar ditampilkan: 50 simbol\n");
    printf("------------------------------------------\n");

    // Rangkuman total
    float net = income - expense;
    if (net >= 0)
        printf("💰 Net Saving : \033[1;32m+Rp%.2f\033[0m\n", net);
    else
        printf("💸 Net Loss   : \033[1;31m-Rp%.2f\033[0m\n", -net);

    printf("==========================================\n");
}

// ======== Buffer cleanup ========
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ======== MAIN PROGRAM ========
int main() {
    int choice;
    login();
    loadFromFile();

    while (1) {
        printf("\n========== 🏦 Fintrix CLI v5.0a ==========\n");
        printf("1. Tambah transaksi\n");
        printf("2. Lihat semua transaksi\n");
        printf("3. Hapus transaksi\n");
        printf("4. Filter berdasarkan kategori\n");
        printf("5. Export ke CSV (Excel)\n");
        printf("6. Lihat grafik\n");
        printf("7. Keluar\n");
        printf("==========================================\n");
        printf("Pilih menu (1-7): ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: showTransactions(); break;
            case 3: deleteTransaction(); break;
            case 4: filterTransactions(); break;
            case 5: exportCSV(); break;
            case 6: showChart(); break;
            case 7:
                printf("👋 Terima kasih telah menggunakan Fintrix!\n");
                saveToFile();
                exit(0);
            default: printf("❌ Pilihan tidak valid!\n");
        }
    }
}