#include <iostream>
#include "expenses.h"

int main() {
    ExpenseList expenses{
            {"2026-06-01", "food", "lunch", 18.50},
            {"2026-06-02", "transport", "bus", 3.20},
            {"2026-06-03", "books", "cpp", 45.00}
    };

    //Uso de Strategy
    auto by_amount= [](const Expense& a, const Expense& b) {
        return a.amount> b.amount;
    };
    sort_with(expenses, by_amount);

    //Uso de Factory Method
    auto factory= make_exporter<JsonExporter>();
    std::cout <<"--- Salida Factory ---\n"<< factory(expenses) << "\n\n";

    //Uso de Decorator
    auto exporter= AuditedExporter<SummaryExporter<CsvExporter>>{
        SummaryExporter<CsvExporter>{
            CsvExporter{}
        }
    };

    std::cout <<"--- Salida Decorator---\n"<< exporter.export_expenses(expenses) << "\n";

    return 0;
}