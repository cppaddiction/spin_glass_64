#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <climits>
#include <cassert>
#include <thread>

using namespace std;

int lattice_linear_x_size;
int lattice_x_size;
int lattice_linear_y_size;
int lattice_y_size;
int lattice_capacity;

int mi = INT_MAX;

vector<vector<string>> LoadHumanLattice(istream& in)
{
	in >> lattice_linear_x_size >> lattice_x_size >> lattice_linear_y_size >> lattice_y_size;
	lattice_capacity = lattice_linear_x_size * lattice_linear_y_size;

	vector<vector<string>> lattice;

	for (int i = 0; i < lattice_y_size; i++)
	{
		vector<string> row;
		for (int j = 0; j < lattice_x_size; j++)
		{
			string str; in >> str; row.push_back(str);
		}
		lattice.push_back(row);
	}

	return lattice;
}

vector<vector<int>> LoadMachineLattice(istream& in)
{
	in >> lattice_linear_x_size >> lattice_x_size >> lattice_linear_y_size >> lattice_y_size;
	lattice_capacity = lattice_linear_x_size * lattice_linear_y_size;

	vector<vector<int>> lattice;

	for (int i = 0; i < lattice_capacity; i++)
	{
		vector<int> row;
		for (int j = 0; j < lattice_capacity; j++)
		{
			int k; in >> k; row.push_back(k);
		}
		lattice.push_back(row);
	}

	return lattice;
}

void PrintHumanLattice(const vector<vector<int>>& lattice, ostream& out)
{
	out << lattice_linear_x_size << ' ' << lattice_x_size << ' ' << lattice_linear_y_size << ' ' << lattice_y_size << '\n';

	int k = 0;
	vector<vector<string>> human_lattice;

	for (int i = 0; i < lattice_y_size; i++)
	{
		vector<string> row;
		for (int j = 0; j < lattice_x_size; j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
			{
				row.push_back("S" + to_string(k++));
			}
			else if (i % 2 != 0 && j % 2 != 0)
			{
				row.push_back("@");
			}
			else
			{
				row.push_back("");
			}
		}
		human_lattice.push_back(row);
	}

	for (int i = 0; i < lattice.size(); i++)
	{
		bool look_up = false;
		bool look_left = false;
		bool look_right = false;
		bool look_down = false;
		for (int j = 0; j < lattice[i].size(); j++)
		{
			if (lattice[i][j] != 0)
			{
				if (!look_up)
				{
					try {
						human_lattice.at((i / lattice_linear_x_size) * 2 - 1).at((i % lattice_linear_x_size) * 2) = (lattice[i][j] == 1 ? "+" : "-");
						look_up = true;
						continue;
					}
					catch (...)
					{
						look_up = true;
					}
				}
				if (!look_left)
				{
					try {
						human_lattice.at((i / lattice_linear_x_size) * 2).at((i % lattice_linear_x_size) * 2 - 1) = (lattice[i][j] == 1 ? "+" : "-");
						look_left = true;
						continue;
					}
					catch (...)
					{
						look_left = true;
					}
				}
				if (!look_right)
				{
					try {
						human_lattice.at((i / lattice_linear_x_size) * 2).at((i % lattice_linear_x_size) * 2 + 1) = (lattice[i][j] == 1 ? "+" : "-");
						look_right = true;
						continue;
					}
					catch (...)
					{
						look_right = true;
					}
				}
				if (!look_down)
				{
					try {
						human_lattice.at((i / lattice_linear_x_size) * 2 + 1).at((i % lattice_linear_x_size) * 2) = (lattice[i][j] == 1 ? "+" : "-");
						look_down = true;
						continue;
					}
					catch (...)
					{
						look_down = true;
					}
				}
			}
		}
	}

	for (int i = 0; i < human_lattice.size(); i++)
	{
		for (int j = 0; j < human_lattice[i].size(); j++)
		{
			out << human_lattice[i][j] << '\t';
		}
		out << '\n';
	}
}

void PrintMachineLattice(const vector<vector<string>>& lattice, ostream& out)
{
	out << lattice_linear_x_size << ' ' << lattice_x_size << ' ' << lattice_linear_y_size << ' ' << lattice_y_size << '\n';

	for (int i = 0; i < lattice.size(); i++)
	{
		for (int j = 0; j < lattice[i].size(); j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
			{
				pair<string, int> up_pair = make_pair("=", -1);
				pair<string, int> left_pair = make_pair("=", -1);
				pair<string, int> right_pair = make_pair("=", -1);
				pair<string, int> down_pair = make_pair("=", -1);
				try
				{
					up_pair = make_pair(lattice.at(i - 1).at(j), stoi(lattice.at(i - 2).at(j).substr(1, lattice.at(i - 2).at(j).size() - 1)));
				}
				catch (...)
				{

				}
				try
				{
					left_pair = make_pair(lattice.at(i).at(j - 1), stoi(lattice.at(i).at(j - 2).substr(1, lattice.at(i).at(j - 2).size() - 1)));
				}
				catch (...)
				{

				}
				try
				{
					right_pair = make_pair(lattice.at(i).at(j + 1), stoi(lattice.at(i).at(j + 2).substr(1, lattice.at(i).at(j + 2).size() - 1)));
				}
				catch (...)
				{

				}
				try
				{
					down_pair = make_pair(lattice.at(i + 1).at(j), stoi(lattice.at(i + 2).at(j).substr(1, lattice.at(i + 2).at(j).size() - 1)));
				}
				catch (...)
				{

				}
				for (int k = 0; k < lattice_capacity; k++)
				{
					if (k == up_pair.second)
					{
						out << (up_pair.first == "+" ? 1 : -1) << ' ';
					}
					else if (k == left_pair.second)
					{
						out << (left_pair.first == "+" ? 1 : -1) << ' ';
					}
					else if (k == right_pair.second)
					{
						out << (right_pair.first == "+" ? 1 : -1) << ' ';
					}
					else if (k == down_pair.second)
					{
						out << (down_pair.first == "+" ? 1 : -1) << ' ';
					}
					else
					{
						out << 0 << ' ';
					}
				}
				out << '\n';
			}
		}
	}
}

vector<vector<string>> GetUpLeftSigns(const vector<vector<string>>& lattice)
{
	vector<vector<string>> up_left_signs;

	for (int i = 0; i < lattice.size(); i++)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[i][0]);
			int j = i - 1;
			while (j >= 0 && abs(i - j) < lattice[j].size())
			{
				row.push_back(lattice[j][abs(i - j)]);
				j--;
			}
			up_left_signs.push_back(row);
		}
	}
	for (int i = 0; i < lattice[lattice.size() - 1].size(); i++)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[lattice.size() - 1][i]);
			int j = lattice.size() - 2;
			while (j >= 0 && i + abs(int(lattice.size() - 1) - j) < lattice[j].size())
			{
				row.push_back(lattice[j][i + abs(int(lattice.size() - 1) - j)]);
				j--;
			}
			up_left_signs.push_back(row);
		}
	}

	return up_left_signs;
}

vector<vector<string>> GetUpRightSigns(const vector<vector<string>>& lattice)
{
	vector<vector<string>> up_right_signs;

	for (int i = 0; i < lattice.size(); i++)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[i][lattice[i].size() - 1]);
			int j = i - 1;
			while (j >= 0 && abs(i - j) < lattice[j].size())
			{
				row.push_back(lattice[j][lattice[j].size() - 1 - abs(i - j)]);
				j--;
			}
			up_right_signs.push_back(row);
		}
	}
	for (int i = lattice[lattice.size() - 1].size() - 1; i >= 0; i--)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[lattice.size() - 1][i]);
			int j = lattice.size() - 2;
			while (j >= 0 && i - abs(int(lattice.size() - 1) - j) >= 0)
			{
				row.push_back(lattice[j][i - abs(int(lattice.size() - 1) - j)]);
				j--;
			}
			up_right_signs.push_back(row);
		}
	}

	return up_right_signs;
}

vector<vector<string>> GetDownLeftSigns(const vector<vector<string>>& lattice)
{
	vector<vector<string>> down_left_signs;

	for (int i = lattice.size() - 1; i >= 0; i--)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[i][0]);
			int j = i + 1;
			while (j < lattice.size() && abs(i - j) < lattice[j].size())
			{
				row.push_back(lattice[j][abs(i - j)]);
				j++;
			}
			down_left_signs.push_back(row);
		}
	}
	for (int i = 0; i < lattice[0].size(); i++)
	{
		if (i % 2 != 0)
		{
			vector<string> row; row.push_back(lattice[0][i]);
			int j = 1;
			while (j < lattice.size() && i + j < lattice[j].size())
			{
				row.push_back(lattice[j][i + j]);
				j++;
			}
			down_left_signs.push_back(row);
		}
	}

	return down_left_signs;
}

vector<vector<string>> GetDownRightSigns(const vector<vector<string>>& up_left_signs)
{
	vector<vector<string>> down_right_signs = up_left_signs;

	reverse(down_right_signs.begin(), down_right_signs.end());

	return down_right_signs;
}

void PrintMatrix(const vector<vector<string>>& matrix, ostream& out)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			out << matrix[i][j] << '\t';
		}
		out << '\n';
	}
}

int ComputePair(int Si, int Sj, string sign)
{
	return sign == "+" ? Si * Sj : -1 * Si * Sj;
}

struct Square {
	Square() = default;
	explicit Square(const vector<int>& numbers, const vector<string>& signs) : numbers_(numbers), signs_(signs) {}
	vector<int> numbers_;
	vector<string> signs_;
};

int ComputeSquare(const Square& square)
{
	return ComputePair(square.numbers_[0], square.numbers_[1], square.signs_[0]) + ComputePair(square.numbers_[1], square.numbers_[2], square.signs_[1]) + ComputePair(square.numbers_[2], square.numbers_[3], square.signs_[2]) + ComputePair(square.numbers_[3], square.numbers_[0], square.signs_[3]);
}

void PrintSquare(const Square& square, ostream& out)
{
	out << "(" << square.numbers_[0] << ") " << square.signs_[0] << " (" << square.numbers_[1] << ")\n";
	out << square.signs_[3] << "       " << square.signs_[1] << "\n";
	out << "(" << square.numbers_[3] << ") " << square.signs_[2] << " (" << square.numbers_[2] << ")\n";
}

unordered_map<int, vector<Square>> GetPossibleUnique2x2SquareConfigurationsByEnergy()
{
	unordered_map<int, vector<Square>> possible_unique_2x2_square_configurations_by_energy;

	for (int i : {1, -1})
	{
		for (int j : {1, -1})
		{
			for (int k : {1, -1})
			{
				for (int t : {1, -1})
				{
					for (string sign1 : { "+", "-" })
					{
						for (string sign2 : { "+", "-" })
						{
							for (string sign3 : { "+", "-" })
							{
								for (string sign4 : { "+", "-" })
								{
									vector<int> numbers{ i, j, k, t };
									vector<string> signs{ sign1, sign2, sign3, sign4 };
									possible_unique_2x2_square_configurations_by_energy[ComputeSquare(Square{ numbers, signs })].emplace_back(numbers, signs);
								}
							}
						}
					}
				}
			}
		}
	}

	return possible_unique_2x2_square_configurations_by_energy;
}

void PrintSquaresWithSpecificEnergy(const unordered_map<int, vector<Square>>& squares_by_energy, int energy, ostream& out)
{
	const auto& squares = squares_by_energy.at(energy);
	for (const auto& square : squares)
	{
		PrintSquare(square, out);
		out << '\n' << energy << '\n' << '\n';
	}
}

unsigned long long IntPow(int x, int y)
{
	unsigned long long result = 1;

	for (int i = 0; i < y; i++)
	{
		result *= x;
	}

	return result;
}

void GetConfiguration(unsigned long long number, vector<int>& configuration)
{
	for (int i = 0; i < configuration.size(); i++)
	{
		configuration[configuration.size() - 1 - i] = number & 1 ? 1 : -1;
		number >>= 1;
	}
}

vector<int> MergeLayers(const vector<int>& up_layer, const vector<int>& down_layer, bool reached_down_left_corner_layer)
{
	vector<int> merged_layers;

	if (!reached_down_left_corner_layer)
	{
		for (int i = 0; i < down_layer.size(); i++)
		{
			merged_layers.push_back(down_layer[i]);
			try
			{
				merged_layers.push_back(up_layer.at(i));
			}
			catch (...)
			{

			}
		}
	}
	else
	{
		for (int i = 0; i < up_layer.size(); i++)
		{
			merged_layers.push_back(up_layer[i]);
			try
			{
				merged_layers.push_back(down_layer.at(i));
			}
			catch (...)
			{

			}
		}
	}

	return merged_layers;
}

int ComputeMergedLayers(const vector<int>& merged_layers, const vector<string>& signs)
{
	int result = 0;

	for (int i = 0; i < signs.size(); i++)
	{
		result += ComputePair(merged_layers[i], merged_layers[i + 1], signs[i]);
	}

	return result;
}

pair<int, vector<vector<int>>> GetPossibleUniqueCurrentLayerNextConfigurationsWithMinimumEnergy(int current_layer, int down_left_corner_layer, const vector<int>& current_layer_configuration, const vector<vector<string>>& signs)
{
	int minimum_energy = INT_MAX;
	unordered_map<int, vector<vector<int>>>	possible_unique_current_layer_next_configurations;

	int current_layer_next_configurations_size = signs[current_layer].size() - current_layer_configuration.size() + 1;

	if (current_layer_next_configurations_size > 64)
	{
		throw out_of_range("maximum diagonal size exceeded");
	}

	unsigned long long number = IntPow(2, current_layer_next_configurations_size) - 1;

	for (unsigned long long i = 0; i <= number; i++)
	{
		vector<int> current_layer_next_configuration(current_layer_next_configurations_size);
		GetConfiguration(i, current_layer_next_configuration);

		auto merged_layers_energy = ComputeMergedLayers(MergeLayers(current_layer_configuration, current_layer_next_configuration, current_layer >= down_left_corner_layer ? true : false), signs[current_layer]);
		possible_unique_current_layer_next_configurations[merged_layers_energy].push_back(current_layer_next_configuration);

		if (merged_layers_energy < minimum_energy)
		{
			minimum_energy = merged_layers_energy;
		}

		if (i == ULLONG_MAX)
		{
			break;
		}
	}

	return make_pair(minimum_energy, possible_unique_current_layer_next_configurations[minimum_energy]);
}

vector<vector<string>> GetState(const vector<vector<int>>& numbers, const vector<vector<string>>& signs, bool reversed_state)
{
	vector<vector<string>> state = reversed_state ? vector<vector<string>>(lattice_x_size, vector<string>(lattice_y_size, "")) : vector<vector<string>>(lattice_y_size, vector<string>(lattice_x_size, ""));

	int k_numbers = 0;
	int k_signs = 0;

	for (int i = 0; i < state.size(); i++)
	{
		if (i % 2 == 0)
		{
			int t = 0;
			state[i][0] = to_string(numbers[k_numbers][t++]);
			int j = i - 1;

			while (j >= 0 && abs(i - j) < state[j].size())
			{
				state[j][abs(i - j)] = j % 2 != 0 && abs(i - j) % 2 != 0 ? "@" : to_string(numbers[k_numbers][t++]);
				j--;
			}

			k_numbers++;
		}
		else
		{
			int t = 0;
			state[i][0] = signs[k_signs][t++];
			int j = i - 1;

			while (j >= 0 && abs(i - j) < state[j].size())
			{
				state[j][abs(i - j)] = signs[k_signs][t++];
				j--;
			}

			k_signs++;
		}
	}
	for (int i = 1; i < state[state.size() - 1].size(); i++)
	{
		if (i % 2 == 0)
		{
			int t = 0;
			state[state.size() - 1][i] = to_string(numbers[k_numbers][t++]);
			int j = state.size() - 2;

			while (j >= 0 && i + abs(int(state.size() - 1) - j) < state[j].size())
			{
				state[j][i + abs(int(state.size() - 1) - j)] = j % 2 != 0 && i + abs(int(state.size() - 1) - j) % 2 != 0 ? "@" : to_string(numbers[k_numbers][t++]);
				j--;
			}

			k_numbers++;
		}
		else
		{
			int t = 0;
			state[state.size() - 1][i] = signs[k_signs][t++];
			int j = state.size() - 2;

			while (j >= 0 && i + abs(int(state.size() - 1) - j) < state[j].size())
			{
				state[j][i + abs(int(state.size() - 1) - j)] = signs[k_signs][t++];
				j--;
			}

			k_signs++;
		}
	}

	return state;
}

int ComputeState(const vector<vector<string>>& state)
{
	int result = 0;

	for (int i = 0; i < state.size(); i++)
	{
		vector<int> horizontal_merged_layers;
		vector<string> horizontal_merged_signs;

		for (int j = 0; j < state[i].size(); j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
			{
				horizontal_merged_layers.push_back(stoi(state[i][j]));
			}
			if (i % 2 == 0 && j % 2 != 0)
			{
				horizontal_merged_signs.push_back(state[i][j]);
			}
		}

		result += ComputeMergedLayers(horizontal_merged_layers, horizontal_merged_signs);
	}

	for (int j = 0; j < state[0].size(); j++)
	{
		vector<int> vertical_merged_layers;
		vector<string> vertical_merged_signs;

		for (int i = 0; i < state.size(); i++)
		{
			if (i % 2 == 0 && j % 2 == 0)
			{
				vertical_merged_layers.push_back(stoi(state[i][j]));
			}
			if (i % 2 != 0 && j % 2 == 0)
			{
				vertical_merged_signs.push_back(state[i][j]);
			}
		}

		result += ComputeMergedLayers(vertical_merged_layers, vertical_merged_signs);
	}

	return result;
}

class Node
{
public:
	Node() = default;

	explicit Node(int current_layer, int current_layer_energy, const vector<vector<int>>& current_layer_configurations, const vector<vector<int>>& current_layer_parents_stack, const vector<vector<string>>& signs) : current_layer_(current_layer), current_layer_energy_(current_layer_energy), current_layer_configurations_(current_layer_configurations), current_layer_parents_stack_(current_layer_parents_stack), signs_(signs) {}

	void BuildTree(int max_layer, int down_left_corner_layer, bool destroy_odd_children, bool reversed_state, ostream& out)
	{
		if (current_layer_ != max_layer)
		{
			int minimum_energy = INT_MAX;
			vector<Node> current_layer_best_children;

			for (const auto& current_layer_configuration : current_layer_configurations_)
			{
				auto child_energy_and_configurations = GetPossibleUniqueCurrentLayerNextConfigurationsWithMinimumEnergy(current_layer_, down_left_corner_layer, current_layer_configuration, signs_);
				auto child = Node(current_layer_ + 1, current_layer_energy_ + child_energy_and_configurations.first, child_energy_and_configurations.second, current_layer_parents_stack_, signs_);

				child.AddParent(current_layer_configuration);

				if (!destroy_odd_children)
				{
					current_layer_children_.push_back(child);
					current_layer_children_[current_layer_children_.size() - 1].BuildTree(max_layer, down_left_corner_layer, destroy_odd_children, reversed_state, out);
					current_layer_children_.pop_back();
				}
				else
				{
					if (current_layer_energy_ + child_energy_and_configurations.first <= minimum_energy)
					{
						minimum_energy = current_layer_energy_ + child_energy_and_configurations.first;
						current_layer_best_children.push_back(child);
					}
				}
			}

			if (destroy_odd_children)
			{
				for (const auto& current_layer_best_child : current_layer_best_children)
				{
					if (current_layer_best_child.GetEnergy() == minimum_energy)
					{
						current_layer_children_.push_back(current_layer_best_child);
						current_layer_children_[current_layer_children_.size() - 1].BuildTree(max_layer, down_left_corner_layer, destroy_odd_children, reversed_state, out);
						current_layer_children_.pop_back();
					}
				}
			}

			if (current_layer_ == 0)
			{
				mi = INT_MAX;
			}
		}
		else
		{
			if (current_layer_energy_ <= mi)
			{
				mi = current_layer_energy_;

				for (const auto& current_layer_configuration : current_layer_configurations_)
				{
					vector<vector<int>> numbers = current_layer_parents_stack_;
					numbers.push_back(current_layer_configuration);

					auto state = GetState(numbers, signs_, reversed_state);
					assert(ComputeState(state) == current_layer_energy_);

					PrintMatrix(state, out);
					out << '\n' << current_layer_energy_ << '\n' << '\n';
				}
			}
		}
	}

	void AddParent(const vector<int>& parent)
	{
		current_layer_parents_stack_.push_back(parent);
	}

	int GetEnergy() const { return current_layer_energy_; }

private:
	int current_layer_ = 0;
	int current_layer_energy_ = 0;
	vector<vector<int>> current_layer_configurations_;
	vector<vector<int>> current_layer_parents_stack_;
	vector<vector<string>> signs_;
	vector<Node> current_layer_children_;
};

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	auto lattice = LoadHumanLattice(in);
	PrintMachineLattice(lattice, out);

	in.close();
	out.close();

	auto up_left_signs = GetUpLeftSigns(lattice);
	auto up_right_signs = GetUpRightSigns(lattice);
	auto down_left_signs = GetDownLeftSigns(lattice);
	auto down_right_signs = GetDownRightSigns(up_left_signs);

	thread thread1([&]() {
		ofstream out_up_left("output_up_left.txt");
		auto up_left_tree = Node(0, 0, { {1}, {-1} }, {}, up_left_signs);
		up_left_tree.BuildTree(up_left_signs.size(), lattice_linear_y_size - 1, true, false, out_up_left);
		out_up_left.close();
		});

	thread thread2([&]() {
		ofstream out_up_right("output_up_right.txt");
		auto up_right_tree = Node(0, 0, { {1}, {-1} }, {}, up_right_signs);
		up_right_tree.BuildTree(up_right_signs.size(), lattice_linear_y_size - 1, true, false, out_up_right);
		out_up_right.close();
		});

	thread thread3([&]() {
		ofstream out_down_left("output_down_left.txt");
		auto down_left_tree = Node(0, 0, { {1}, {-1} }, {}, down_left_signs);
		down_left_tree.BuildTree(down_left_signs.size(), lattice_linear_y_size - 1, true, false, out_down_left);
		out_down_left.close();
		});

	thread thread4([&]() {
		ofstream out_down_right("output_down_right.txt");
		auto down_right_tree = Node(0, 0, { {1}, {-1} }, {}, down_right_signs);
		down_right_tree.BuildTree(down_right_signs.size(), lattice_linear_x_size - 1, true, true, out_down_right);
		out_down_right.close();
		});

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();

	return 0;
}