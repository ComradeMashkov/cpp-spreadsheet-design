#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
    Cell();
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

private:
    SheetInterface& sheet_interface_;

    set<Cell*> dependent_cells_;
    set<Cell*> referenced_cells_;

    mutable std::optional<Value> cache_;

    class Impl {
    public:
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;

        virtual std::vector<Position> GetReferencedCells() const = 0;
        
        virtual ~Impl() = default;
    };
    
    class EmptyImpl final : public Impl {
    public:
        Value GetValue() const override;
        std::string GetText() const override;
    };
    
    class TextImpl final : public Impl {
    public:
        explicit TextImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;
        
    private:
        std::string text_;
    };
    
    class FormulaImpl final : public Impl {
    public:
        explicit FormulaImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;

        std::vector<Position> GetReferencedCells() const override;
        
    private:
        std::unique_ptr<FormulaInterface> formula_interface_;
    };
    
    std::unique_ptr<Impl> impl_;

    bool HasCircularDependency(Impl& impl);
    void InvalidateCache();
    void RefreshCells();
};