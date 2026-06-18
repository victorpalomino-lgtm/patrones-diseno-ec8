#ifndef PATRONES_DISENO_EC8_EXPENSES_H
#define PATRONES_DISENO_EC8_EXPENSES_H


#include <string>
#include <vector>
#include <functional>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <tuple>

// Estructura base
struct Expense {
    std::string date;
    std::string category;
    std::string detail;
    double amount{};
};

using ExpenseList= std::vector<Expense>;

template<class T>
concept ExpenseExporter= requires (T exporter, const ExpenseList& expenses) {
    { exporter.export_expenses(expenses) } -> std::same_as<std::string>;
};
using ExportFunction= std::function<std::string(const ExpenseList&)>;

//STRATEGY
template<class R, class Cmp>
concept SortStrategy=
    std::ranges::random_access_range<R> &&
    std::sortable<std::ranges::iterator_t<R>, Cmp>;

void sort_with(auto& values, auto cmp) requires SortStrategy<decltype(values), decltype(cmp)> {
    std::ranges::sort(values, cmp);
}

//FACTORY METHOD
template<ExpenseExporter E, class... Args>
ExportFunction make_exporter(Args&&... args) {
    return [exporter= E(std::forward<Args>(args)...)](const ExpenseList& expenses) {
        return exporter.export_expenses(expenses);
    };
}

//EXPORTADORES BASE
struct CsvExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::string out= "date, category, detail, amount\n";
        for (const auto& e : expenses) {
            out+= e.date+ ", "+ e.category+ ", "+ e.detail+ ", "+ std::to_string(e.amount)+ "\n";
        }
        return out;
    }
};

struct JsonExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::string out= "[\n";
        for (size_t i = 0; i < expenses.size(); ++i) {
            out+= "  {\n";
            out+= "    \"date\": \"" + expenses[i].date+ "\",\n";
            out+= "    \"category\": \""+ expenses[i].category+ "\",\n";
            out+= "    \"detail\": \""+ expenses[i].detail+ "\",\n";
            out+= "    \"amount\": " + std::to_string(expenses[i].amount)+ "\n";
            out+= "  }" + std::string(i== expenses.size()-1 ? "" : ",")+ "\n";
        }
        out+= "]\n";
        return out;
    }
};

struct TextExporter {
    std::string export_expenses(const ExpenseList& expenses) const {
        std::string out= "--- Gastos ---\n";
        for (const auto& e : expenses) {
            out+= e.date+" | " +e.category +" | " +e.detail +" | " + std::to_string(e.amount)+"\n";
        }
        return out;
    }
};

//DECORATOR
template<ExpenseExporter Inner>
struct AuditedExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const {
        return "[audit] Exporting "+ std::to_string(expenses.size())+" expenses.\n" +
               inner.export_expenses(expenses);
    }
};

template<ExpenseExporter Inner>
struct SummaryExporter {
    Inner inner;
    std::string export_expenses(const ExpenseList& expenses) const {
        double total = 0;
        for (const auto& e : expenses) total+= e.amount;
        return inner.export_expenses(expenses)+ "\n[total]: "+ std::to_string(total)+"\n";
    }
};


#endif //PATRONES_DISENO_EC8_EXPENSES_H