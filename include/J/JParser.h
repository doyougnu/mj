#ifndef J_JPARSER_H
#define J_JPARSER_H

#include "mlir/IR/Builders.h"
#include "mlir/IR/Value.h"
#include <string>
#include <vector>
#include <memory>

namespace j {

enum class Kind { Noun, Verb, Adverb, EOF_TK };

struct Token {
    Kind kind;
    std::string text;
};

class JParser {
public:
    JParser(mlir::MLIRContext *ctx, std::vector<Token> tokens);

    // The entry point to get a single MLIR Value (the result of the J expr)
    mlir::Value parseExpr(int min_bp = 0);

private:
    struct PrattRule {
        using NudFn = mlir::Value (JParser::*)();
        using LedFn = mlir::Value (JParser::*)(mlir::Value);
        NudFn nud;
        LedFn led;
        int bp;
    };

    static const PrattRule& getRule(Kind kind);

    // Parse functions
    mlir::Value parseNoun();    // Nud: handles numbers/arrays
    mlir::Value parseMonad();   // Nud: handles -y
    mlir::Value parseDyad(mlir::Value lhs); // Led: handles x + y
    mlir::Value parseAdv(mlir::Value lhs);  // Led: handles +/

    // Helpers
    Token consume();
    Token peek();

    std::vector<Token> tokens;
    size_t pos = 0;
    mlir::OpBuilder builder;
    Token last_token;
};

} // namespace j

#endif
