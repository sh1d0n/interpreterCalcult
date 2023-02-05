#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

//dimensions of the transition table
const int linesT = 10;
const int columnsT = 11;

//dimensions of the state machine
const int linesS = 13;
const int columnsS = 9;

typedef  std::unordered_map<char,int> MyMap;

void solution();
double operationExecution(char op, double fo, double so);
double operationExecution(char op, double operand);
void helpMenu();

bool my_isdigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch));
}

void fileOpening(std::unordered_map<char, MyMap>& map, std::unordered_map<int,MyMap>& map2)
{
    std::ifstream input("input.in");
    if (input.is_open())
    {
        for(int i = 0; i < linesT; i++)
        {
            char key1;
            input >> key1;
            for(int j = 0; j < columnsT; j++)
            {
                char key2;
                input >> key2;
                int count;
                input >> count;
                map[key1][key2] = count;
            }
        }

        for(int i = 0; i < linesS; i++)
        {
            int key1;
            input >> key1;
            for(int j = 0; j < columnsS; j++)
            {
                char key2;
                input >> key2;
                int count;
                input >> count;
                map2[key1][key2] = count;
            }
        }
    }
    else
    {
        std::cout << "File opening error\n\n";
        system("pause");
        exit(0);
    }
    input.close();
}

void startScreen()
{
    system("cls");
    std::cout << std::setw(20) << " " << "Calculator!\n\n";
    std::cout << "1. Start\n";
    std::cout << "2. Help\n";
    std::cout << "3. Exit\n\n";

    bool mistake = false;

    do
    {
        std::cout << ">> ";
        int choice;
        std::cin >> choice;

        switch(choice)
        {
            case 1:
                solution();
                mistake = false;
                break;
            case 2:
                helpMenu();
                mistake = false;
                break;
            case 3:
                exit(0);
            default:
                mistake = true;
                std::cout << "\nThere is no such option! Try again...\n\n";
                break;
        }
    }while(mistake);
}

void helpMenu()
{
    system("cls");
    std::cout << std::setw(20) << " " << "Help Menu!\n\n";
    std::cout << "1. The expression must be written without spaces! \n  For example: 25.84+56/2\n";
    std::cout << "2. Characters used when entering an expression:\n";
    std::cout << "  2.1. '.' - used to separate integer and fractional parts of a number\n";
    std::cout << "  2.2. '+' - used to add numbers \n";
    std::cout << "  2.3. '-' - used to subtract one number from another\n";
    std::cout << "  2.4. '*' - used to multiply numbers \n";
    std::cout << "  2.5. '/' - used to divide numbers \n";
    std::cout << "  2.6. '(' and ')' - used to enclose an expression in brackets \n";
    std::cout << "  2.7. 'sqrt' - the square root, used to extract the square root of the expression \n";
    std::cout << "  2.8. 'ln' - the natural logarithm, used to find the natural logarithm of the expression\n";
    std::cout << "  2.9. 'sin' - sine, used to find the sine of the expression\n";
    std::cout << "  2.10. 'cos' - cosine, used to find the cosine of the expression\n\n\n";

    std::cout << "1. Main menu\n\n";
    std::cout << ">> ";
    int input = 0;
    std::cin >> input;

    switch (input)
    {
        case 1:
            startScreen();
        default:
            std::cout << "\n\n" << "There is no such option! Try again..." << "\n\n>> ";
            std::cin >> input;
    }
}

bool workOnNumber(bool isLeft, std::string& number, char lI, std::stack<double>& operands)
{
    if (!isLeft && number.empty() && lI == '-')
    {
        number += lI;
        return true;
    }
    else if (!number.empty() && lI == '.')
    {
        number += lI;
        return true;
    }
    else if (number.empty() && lI == '.')
    {
        std::cout << "\nAn error has been made in the expression! Before '.' no numbers!\n";
        system("pause");
        solution();
        exit(0);
    }
    else if (!number.empty())
    {
        if (number.back() == '.')
        {
            std::cout << "\nAn error has been made in the expression! After the character '.' no numbers!\n";
            system("pause");
            solution();
            exit(0);
        }
        else
        {
            operands.push(std::stod(number));
            number.clear();
        }
    }
    return false;
}

void functionAssembly(char lI, std::unordered_map<int,MyMap> machine, int& m, std::string& operationName, std::vector<int> F, char& operationReduction)
{
    if (machine.at(m).at(lI) != 12)
    {
        m = machine.at(m).at(lI);
        operationName += lI;
    }
    else
    {
        std::cout << "\nAn error has been made in the expression! Operator " << operationName
                  << " does not exist\n";
        system("pause");
        solution();
        exit(0);
    }

    for (size_t j = 0; j < F.size(); j++)
    {
        if (m == F[j])
        {
            if (operationName == "sin")
            {
                operationReduction = 's';
            }
            else if (operationName == "cos")
            {
                operationReduction = 'c';
            }
            else if (operationName == "sqrt")
            {
                operationReduction = 'r';
            }
            else if (operationName == "ln")
            {
                operationReduction = 'l';
            }
            operationName.clear();
            m = 0;
            break;
        }
    }
}

std::string doubleToString(double number)
{
    std::string resultNumber = std::to_string(number);
    std::vector<char> text;
    for(size_t i = 0; i < resultNumber.size(); i++)
    {
        text.push_back(resultNumber[i]);
    }

    for(size_t i = text.size() - 1; i > 0; i--)
    {
        if(text[i] == '0' || text[i] == '.')
        {
            text.pop_back();
        }
        else
        {
            break;
        }
    }

    resultNumber.clear();

    for(size_t i = 0; i < text.size(); i++)
    {
        resultNumber += text[i];
    }

    return resultNumber;
}

void solutionSteps(char topOfOS, std::stack<double>& operands, std::vector<std::string>& savedSolution, std::stack<char>& operations)
{
    double firstOperand;
    std::string solutionStep;
    if(topOfOS == '+' || topOfOS == '*' || topOfOS == '/' || topOfOS == '-')
    {
        double secondOperand;
        secondOperand = operands.top();
        operands.pop();
        firstOperand = operands.top();
        operands.pop();
        operands.push(operationExecution(topOfOS, firstOperand, secondOperand));
        solutionStep = doubleToString(firstOperand);
        solutionStep += " ";
        solutionStep += topOfOS;
        solutionStep += " ";
        solutionStep += doubleToString(secondOperand);
        solutionStep += " = ";
        solutionStep += doubleToString(operands.top());
    }
    else
    {
        firstOperand = operands.top();
        operands.pop();
        operands.push(operationExecution(topOfOS, firstOperand));
        if(topOfOS == 's')
        {
            solutionStep = "sin";
        }
        else if(topOfOS == 'c')
        {
            solutionStep = "cos";
        }
        else if(topOfOS == 'r')
        {
            solutionStep = "sqrt";
        }
        else if(topOfOS == 'l')
        {
            solutionStep = "ln";
        }
        solutionStep += doubleToString(firstOperand);
        solutionStep += " = ";
        solutionStep += doubleToString(operands.top());
    }
    savedSolution.push_back(solutionStep);
    operations.pop();
}

void solution()
{
    std::stack<char> operations;
    std::stack<double> operands;
    std::vector<int> F{7, 8, 10, 11};
    std::unordered_map<char, MyMap> transitionTable;
    std::unordered_map<int, MyMap> stateMachine;

    fileOpening(transitionTable, stateMachine);


    system("cls");
    std::cout << "Enter the expression\n>>";
    std::string lineInput;
    std::cin >> lineInput;

    std::string number;
    int m = 0;
    std::string operationName;
    char operationReduction;
    bool isLeftInTheCurrentIteration = false;
    std::vector<std::string> savedSolution;
    for(size_t i = 0; i < lineInput.size(); i++)
    {
        char lI = lineInput[i];
        if(my_isdigit(lI))
        {
            number+=lI;
        }
        else
        {
            if(workOnNumber(isLeftInTheCurrentIteration, number, lI, operands))
            {
                continue;
            }

            if (lI == 's' || lI == 'i' || lI == 'n' || lI == 'q' || lI == 'r' || lI == 't' || lI == 'c' || lI == 'o' ||
                lI == 'l')
            {
                functionAssembly(lI, stateMachine, m, operationName, F, operationReduction);
            }

            if(lI == '+' || lI == '-' || lI == '*' || lI == '(' || lI == ')' || lI == '/')
            {
                operationReduction = lI;
            }

            char topOfOS; //top of operations stack
            if(operations.empty())
            {
                topOfOS = 'e';
            }
            else
            {
                topOfOS = operations.top();
            }

            if(operationReduction != NULL)
            {
                switch(transitionTable.at(topOfOS).at(operationReduction))
                {
                    case 2:
                        operations.push(operationReduction);
                        operationReduction = NULL;
                        isLeftInTheCurrentIteration = false;
                        break;
                    case 3:
                        std::cout << "\nAn error has been made in the expression! Check the correctness of the operator "
                                  << operationReduction << "\n";
                        system("pause");
                        solution();
                        exit(0);
                    case 5:
                        solutionSteps(topOfOS, operands, savedSolution, operations);
                        i--;
                        isLeftInTheCurrentIteration = true;
                        operationReduction = NULL;
                        break;
                    case 6:
                        operations.pop();
                        isLeftInTheCurrentIteration = true;
                        operationReduction = NULL;
                        break;
                }
            }
        }
    }

    if(!number.empty())
    {
        if (number.back() == '.')
        {
            std::cout << "\nAn error has been made in the expression! After the character '.' no numbers!\n";
            system("pause");
            solution();
            exit(0);
        }
        else
        {
            operands.push(std::stod(number));
            number.clear();
        }
    }

    while(!operations.empty())
    {
        if(operations.top() == ')')
        {
            operations.pop();
        }
        else
        {
            std::string solutionStep;
            double firstOperand;
            if(operations.top() == '+' || operations.top() == '*' || operations.top() == '/' || operations.top() == '-')
            {
                double secondOperand;
                secondOperand = operands.top();
                operands.pop();
                firstOperand = operands.top();
                operands.pop();
                operands.push(operationExecution(operations.top(), firstOperand, secondOperand));
                solutionStep = doubleToString(firstOperand);
                solutionStep += " ";
                solutionStep += operations.top();
                solutionStep += " ";
                solutionStep += doubleToString(secondOperand);
                solutionStep += " = ";
                solutionStep += doubleToString(operands.top());
            }
            else
            {
                firstOperand = operands.top();
                operands.pop();
                operands.push(operationExecution(operations.top(), firstOperand));
                if(operations.top() == 's')
                {
                    solutionStep = "sin";
                }
                else if(operations.top() == 'c')
                {
                    solutionStep = "cos";
                }
                else if(operations.top() == 'r')
                {
                    solutionStep = "sqrt";
                }
                else if(operations.top() == 'l')
                {
                    solutionStep = "ln";
                }
                solutionStep += doubleToString(firstOperand);
                solutionStep += " = ";
                solutionStep += doubleToString(operands.top());
            }
            savedSolution.push_back(solutionStep);
            operations.pop();
        }
    }

    system("cls");
    std::cout << "Enter the expression\n>>" << lineInput << " = " << doubleToString(operands.top());


    bool again = false;

    do
    {
        std::cout << "\n\n";
        std::cout << "\n1)Main menu";
        std::cout << "\n2)Show solution";
        std::cout << "\n3)Other expression";
        std::cout << "\n4)Exit";
        std::cout << "\n\n>> ";
        int choice;
        std::cin >> choice;

        switch(choice)
        {
            case 1:
                startScreen();
                again = false;
                break;
            case 2:
                std::cout << "\n";
                for(size_t i = 0; i < savedSolution.size(); i++)
                {
                    std::cout << "\nStep " << i+1 << ": " << savedSolution[i];
                }
                again = true;
                break;
            case 3:
                solution();
                exit(0);
            case 4:
                exit(0);
            default:
                again = true;
                std::cout << "\nThere is no such option! Try again...\n\n";
                break;
        }
    }while(again);
}

double operationExecution(char op, double fo, double so)
{
    double result;
    if(op == '+')
    {
        result = fo + so;
    }
    else if(op == '*')
    {
        result = fo * so;
    }
    else if(op == '/')
    {
        result = fo / so;
    }
    else if(op == '-')
    {
        result = fo - so;
    }

    return result;
}

double operationExecution(char op, double operand)
{
    double result;
    if(op == 'r')
    {
        result = sqrt(operand);
    }
    else if(op == 'l')
    {
        result = log(operand);
    }
    else if(op == 's')
    {
        result = sin(operand);
    }
    else if(op == 'c')
    {
        result = cos(operand);
    }

    return result;
}


int main()
{
    startScreen();


    system("pause");
}
