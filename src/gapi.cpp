/*
 * gapi.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: @DaeronSyriath
 */

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <ctime>


class TablePosition{
public:
	int x=0;
	int y=0;
	TablePosition(int const x, int const y): x(x), y(y){};
	double distanceTo(TablePosition const & target);
};

double TablePosition::distanceTo(TablePosition const & target){
	return sqrt(pow(target.x - x, 2) + pow(target.y -y, 2));
}


class TableModel{
public:
	TablePosition position;
	int radius = 20;
	double range = 25.6;

	TableModel(int const & x, int const & y, int const & radius ):
			position(TablePosition(x,y)), radius(radius){};

	double distanceTo(TableModel const & target);
	double distanceTo(TablePosition const & target);
};

double TableModel::distanceTo(TableModel const & target){
	return position.distanceTo(target.position) - radius - target.radius;
}


double TableModel::distanceTo(TablePosition const & target){
	return position.distanceTo(target) - radius;
}


TablePosition randomMoveFor(TableModel const & model){
	return TablePosition(model.position.x - 77 + rand()%155, model.position.y -77 + rand()%155);
}




class PileInChromosome{
public:
	std::vector<TableModel>& models;
	std::vector<TableModel>& opponents;
	std::vector<TablePosition> moves;
	PileInChromosome(std::vector<TableModel> & models, std::vector<TableModel> & opponents);

	double getFitness();

	void recombineFrom(PileInChromosome mother, PileInChromosome father);

private:
	void calculateFitness();
	double fitness = 0;
	bool know_fitness = false;
};

PileInChromosome::PileInChromosome(std::vector<TableModel> & models, std::vector<TableModel> & opponents) :models(models), opponents(opponents){
	moves.reserve(models.size());
	for(unsigned int i = 0; i<models.size(); i++ ){
		moves.push_back(randomMoveFor(models.at(i)));
	}
}

double PileInChromosome::getFitness(){
		calculateFitness();
		return fitness;
}

void PileInChromosome::calculateFitness(){
	fitness = 0;
	for(unsigned int i=0; i<moves.size() ; i++){
		double smallest_distance = 400;
		for(unsigned int j=0 ;j<opponents.size();j++){
			smallest_distance = std::min(smallest_distance,
						std::max(0.0 , opponents.at(j).distanceTo(moves.at(i)) - models.at(i).radius)
						);
		}
		fitness+= smallest_distance;
	}
}


void PileInChromosome::recombineFrom(PileInChromosome mother, PileInChromosome father){
	for(unsigned int i=0; i< moves.size();i++){
		if((rand()%2) ==0 ){
			moves.at(i) = mother.moves.at(i);
		} else {
			moves.at(i) = father.moves.at(i);
		}
	}
}




struct PositionReference{
	int position;
	double fitness;
	PositionReference(int position, double fitness):position(position), fitness(fitness){};

};

void printModelAsSvg(TableModel const & model, bool range, std::string type){
	if(range){
		std::cout<<"<circle cx=\""<<model.position.x<<"\" cy=\""<<model.position.y<<"\" r=\""<<model.radius/2 + model.range<<"\" class=\"range_marker\"/>"<<std::endl;
	}
	std::cout<<"<circle cx=\""<<model.position.x<<"\" cy=\""<<model.position.y<<"\" r=\""<<model.radius/2<<"\" class=\""<<type<<"\"/>"<<std::endl;
}

void printModelAtPositionAsSvg(TableModel const & model, TablePosition const & position, bool range, std::string type){
	if(range){
		std::cout<<"<circle cx=\""<<position.x<<"\" cy=\""<<position.y<<"\" r=\""<<model.radius/2 + model.range<<"\" class=\"range_marker\"/>"<<std::endl;
	}
	std::cout<<"<circle cx=\""<<position.x<<"\" cy=\""<<position.y<<"\" r=\""<<model.radius/2<<"\" class=\""<<type<<"\"/>"<<std::endl;
}


void printMoveLineAsSvg(TableModel const & start, TablePosition const & move){
	std::cout<<"<line x1=\""<<start.position.x<<"\" y1=\""<<start.position.y
			<<"\" x2=\""<<move.x<<"\" y2=\""<<move.y<<"\" class=\"move\"/>"<<std::endl;
}

void printChromosomeAsSvg(PileInChromosome const & chromosome){
	for(unsigned int i=0; i<chromosome.opponents.size();i++) {
		printModelAsSvg(chromosome.opponents.at(i), false, "opponent");
	}
	for(unsigned int i=0; i<chromosome.models.size();i++) {
		printModelAsSvg(chromosome.models.at(i), false, "starting_position");
		printMoveLineAsSvg(chromosome.models.at(i), chromosome.moves.at(i));
		printModelAtPositionAsSvg(chromosome.models.at(i),chromosome.moves.at(i), true, "moved_position");
	}
}

/*
 *
 * <svg class="" width="100%" viewBox="0 0 400 200" id="best_answer_pile_in_canvas">
 * 	<circle cx="20" cy="40" r="16" class="starting_position"></circle>
 * 	<line x1="20" y1="40" x2="10" y2="93" class="move"></line>
 * 	<circle cx="10" cy="93" r="41.4" class="range_marker"></circle>
 * 	<circle cx="10" cy="93" r="16" class="moved_position"></circle>
 *
 * 	<circle cx="60" cy="40" r="16" class="starting_position"></circle>
 * 	<line x1="60" y1="40" x2="5" y2="72" class="move"></line>
 * 	<circle cx="5" cy="72" r="41.4" class="range_marker"></circle>
 * 	<circle cx="5" cy="72" r="16" class="moved_position"></circle>
 * 	<circle cx="100" cy="40" r="16" class="starting_position"></circle>
 * 	<line x1="100" y1="40" x2="31" y2="46" class="move"></line>
 * 	<circle cx="31" cy="46" r="41.4" class="range_marker"></circle>
 * 	<circle cx="31" cy="46" r="16" class="moved_position"></circle>
 * 	<circle cx="140" cy="40" r="16" class="starting_position"></circle>
 * 	<line x1="140" y1="40" x2="64" y2="48" class="move"></line>
 * 	<circle cx="64" cy="48" r="41.4" class="range_marker"></circle><circle cx="64" cy="48" r="16" class="moved_position"></circle><circle cx="180" cy="40" r="16" class="starting_position"></circle><line x1="180" y1="40" x2="172" y2="112" class="move"></line><circle cx="172" cy="112" r="41.4" class="range_marker"></circle><circle cx="172" cy="112" r="16" class="moved_position"></circle><circle cx="220" cy="40" r="16" class="starting_position"></circle><line x1="220" y1="40" x2="164" y2="49" class="move"></line><circle cx="164" cy="49" r="41.4" class="range_marker"></circle><circle cx="164" cy="49" r="16" class="moved_position"></circle><circle cx="260" cy="40" r="16" class="starting_position"></circle><line x1="260" y1="40" x2="279" y2="58" class="move"></line><circle cx="279" cy="58" r="41.4" class="range_marker"></circle><circle cx="279" cy="58" r="16" class="moved_position"></circle><circle cx="300" cy="40" r="16" class="starting_position"></circle><line x1="300" y1="40" x2="283" y2="108" class="move"></line><circle cx="283" cy="108" r="41.4" class="range_marker"></circle><circle cx="283" cy="108" r="16" class="moved_position"></circle><circle cx="340" cy="40" r="16" class="starting_position"></circle><line x1="340" y1="40" x2="289" y2="-11" class="move"></line><circle cx="289" cy="-11" r="41.4" class="range_marker"></circle><circle cx="289" cy="-11" r="16" class="moved_position"></circle><circle cx="380" cy="40" r="16" class="starting_position"></circle><line x1="380" y1="40" x2="336" y2="16" class="move"></line><circle cx="336" cy="16" r="41.4" class="range_marker"></circle><circle cx="336" cy="16" r="16" class="moved_position"></circle><circle cx="60" cy="150" r="16" class="enemy_position"></circle><circle cx="100" cy="150" r="16" class="enemy_position"></circle><circle cx="140" cy="150" r="16" class="enemy_position"></circle><circle cx="180" cy="150" r="16" class="enemy_position"></circle><circle cx="220" cy="150" r="16" class="enemy_position"></circle><circle cx="260" cy="150" r="16" class="enemy_position"></circle><circle cx="300" cy="150" r="16" class="enemy_position"></circle>
 * 	<circle cx="340" cy="150" r="16" class="enemy_position"></circle>
 * 	</svg>
 *
 *
 *
 *
 */


int main(int argc, char** argv){

	unsigned int population_size = 100;
    std::srand(std::time(NULL));

    double crossover_rate = 0.2;

	TableModel mod1(0,0, 25);
	TableModel mod2(110,110, 32);

	std::vector<TableModel> models = {
				{20,20,25},
				{50,20,25},
				{80,20,25},
				{110,20,25},
				{140,20,25},
				{170,20,25},
				{200,20,25},
				{230,20,25},
				{260,20,25},
				{290,20,25}
	};

	std::vector<TableModel> opponents = {
				{50,120,25},
				{80,120,25},
				{110,120,25},
				{140,120,25},
				{170,120,25},
				{200,120,25},
				{230,120,25},
				{260,120,25}
	};

	std::vector<PileInChromosome> odd_generation;
	std::vector<PileInChromosome> even_generation;
	std::vector<unsigned int> positions;

	std::vector<PileInChromosome>* current_generation = &even_generation;
	std::vector<PileInChromosome>* next_generation = &odd_generation;
	std::vector<PileInChromosome>* swap = current_generation;


	odd_generation.reserve(population_size);
	even_generation.reserve(population_size);
	positions.reserve(population_size);

	for(unsigned int i=0;i<population_size;i++){
		odd_generation.emplace_back(models, opponents);
		even_generation.emplace_back(models, opponents);
		positions.push_back(i);
	}
	/*
	std::cout<< "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
			<< "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" width=\"100%\" viewBox=\"0 0 400 200\" id=\"best_answer_pile_in_canvas\">\n"
			<< "<style>\n\t.starting_position{fill:#00f;}\n\t.moved_position{fill:#0f0;}\n\t.opponent{fill:#f00;}\n\t.range_marker{fill:rgba(255,64,64,0.3);}\n\t.move{stroke:#0f0;stroke-width:1px;}\n</style>" << std::endl;
	printChromosomeAsSvg(population.at(0));
	std::cout<< "</svg>" << std::endl;
*/



	for(int generation=0; generation<5; generation++){
		std::cout<<"Generation: "<<generation<<std::endl;
		/* Sort */
		std::sort(positions.begin(), positions.end(),
					[&current_generation]( int  idx1, int idx2){
						return current_generation->at(idx1).getFitness() < current_generation->at(idx2).getFitness();
		});

		for(unsigned int i=0;i<population_size;i++){
			std::cout<<"["<<i<<"] "<<current_generation->at(positions.at(i)).getFitness()<<std::endl;

		}
		/* Make new population */

		for(unsigned int i=0;i<population_size;i++){

			float pick_mother = crossover_rate;
			float pick_father = crossover_rate;
			for(unsigned int idx_m=0; idx_m+1<population_size;idx_m++){
				if((rand()%1000) > pick_mother) {
					pick_mother = pick_mother * (1 - crossover_rate);
					continue;
				}
				for(unsigned int idx_f=0; idx_f+1<population_size;idx_f++){
					if((rand()%1000)/1000 > pick_father) {
						pick_father = pick_father * (1 - crossover_rate);
						continue;
					}
					std::cout<<i<<" from "<<idx_m<<","<<idx_f<<std::endl;
					next_generation->at(i).recombineFrom(current_generation->at(positions.at(idx_m)),
														current_generation->at(positions.at(idx_f)));
					break;
				}
				break;
			}
		}
		swap = current_generation;
		current_generation = next_generation;
		next_generation = swap;


	}

	std::cout<<"GAPI closing."<<std::endl;
	return 0;
}


