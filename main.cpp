#include <cctype>
#include <climits>
#include <iostream>
#include <string>

struct Urinal
{
	bool available = false;
	Urinal* left = nullptr;
	Urinal* right = nullptr;
	int value = 0;
};

Urinal* GetUrinalConfig(int& count);
void GetUrinalRec(int count, std::string config, Urinal* urinals);
void WeighUrinalValue(Urinal* urinals, int count);
int FindMostDesireableUrinal(Urinal* urinals, int count);
void DisplayResult(Urinal* urinals, int count, int best);
void PresentResultsText(Urinal* urinals, int count);

int main()
{
	int count;
	Urinal* urinals = GetUrinalConfig(count);
	WeighUrinalValue(urinals, count);
	int best = FindMostDesireableUrinal(urinals, count);
	PresentResultsText(urinals, count);
	std::cout << "Result: ";
	DisplayResult(urinals, count, best);

	return 0;
}

Urinal* GetUrinalConfig(int& count)
{
	std::cout 
	<< "Enter urinal configuration.\n"
	<< "'O' for available, 'X' for taken.\n"
	<< "Example: OOOXOOX\n";

	std::string config;
	std::cin >> config;

	count = config.length();

	Urinal* urinals = new Urinal[config.length()];
	GetUrinalRec(config.length() - 1, config, urinals);
	return urinals;
}

void GetUrinalRec(int count, std::string config, Urinal* urinals)
{
	if (count < 0)
		return;

	if (config[count] == 'O' || config[count] == 'o')
		urinals[count].available = true;

	GetUrinalRec(count - 1, config, urinals);
	if (count == 0)
		urinals[count].right = &urinals[count + 1];
	else if (count < config.length() - 1)
	{
		urinals[count].left = &urinals[count - 1];
		urinals[count].right = &urinals[count + 1];
	}
	else
		urinals[count].left = &urinals[count - 1];
}

void WeighUrinalValue(Urinal* urinals, int count)
{
	Urinal* trav = nullptr;
	for (int i = 0; i < count; i++)
	{
		if (!urinals[i].available)
		{
			urinals[i].value -= 9999;
			continue;
		}
		// Checking the urinals to the left of the current one.
		if (urinals[i].left == nullptr && urinals[i].right->available)
			urinals[i].value += 100;
		else if (urinals[i].left != nullptr)
		{
			if (!urinals[i].left->available)
				urinals[i].value -= 50;
			else
			{
				trav = urinals[i].left;
				while (trav != nullptr && trav->available)
				{
					urinals[i].value += 10;
					trav = trav->left;
				}
			}
		}

		// Checking the urinals to the right of the current one.
		if (urinals[i].right == nullptr && urinals[i].left->available)
			urinals[i].value += 100;
		else if (urinals[i].right != nullptr)
		{
			if (!urinals[i].right->available)
				urinals[i].value -= 50;
			else
			{
				trav = urinals[i].right;
				while (trav != nullptr && trav->available)
				{
					urinals[i].value += 10;
					trav = trav->right;
				}
			}
		}
	}
}

int FindMostDesireableUrinal(Urinal* urinals, int count)
{
	int bestValue = INT_MIN;
	int bestIndex = 0;
	for (int i = 0; i < count; i++)
	{
		int val = urinals[i].value;
		if (urinals[i].left != nullptr)
		{
			if (urinals[i].left->available && urinals[i].left->value <= urinals[i].value)
				val += urinals[i].left->value;
			else if (!urinals[i].left->available)
			{
				Urinal* trav = &urinals[i];
				while (trav->left != nullptr && !trav->left->available)
				{
					val -= 50;
					trav = trav->left;
				}
			}
		}

		if (urinals[i].right != nullptr)
		{
			if (urinals[i].right->available && urinals[i].right->value <= urinals[i].value)
				val += urinals[i].right->value;
			else if (!urinals[i].right->available)
			{
				Urinal* trav = &urinals[i];
				while (trav->right != nullptr && !trav->right->available)
				{
					val -= 50;
					trav = trav->right;
				}
			}
		}

		if (val > bestValue)
		{
			bestValue = val;
			bestIndex = i;
		}
	}
	return bestIndex;
}

void DisplayResult(Urinal* urinals, int count, int best)
{
	for (int i = 0; i < count; i++)
	{
		if (i == best)
			std::cout << "\033[92mO";
		else if (urinals[i].available)
			std::cout << "\033[91mO";
		else
			std::cout << "\033[96mX";
	}
}

void PresentResultsText(Urinal* urinals, int count)
{
	for (int i = 0; i < count; i++)
	{
		std::cout << "Urinal #" << i << " = " << urinals[i].value << '\n';
	}
}