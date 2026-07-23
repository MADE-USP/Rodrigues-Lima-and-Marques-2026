/******************************************************************************

	K_SECTOR OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the capital-good sector objects in the Made-Green model
	are coded below.

 ******************************************************************************/

//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 1: PRODUCTIVITY AVERAGES----------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("productivity_k_avg")
		/*UNIT: number of capital goods
		
		Average productivity in the K goods sector*/

		v[0] = AVE("_productivity_k");

	RESULT(v[0])


	EQUATION("green_productivity_k_avg")
		/*UNIT: number of emissions per capital good produced
		
		Average green productivity in the K goods sector*/

		v[0] = AVE("_green_productivity_k");

	RESULT(v[0])


	EQUATION("productivity_c_new_avg")
		/*UNIT: number of capital goods
		
		Average productivity in the C goods sector for new capital goods */

		v[0] = AVE("_productivity_c_new");

	RESULT(v[0])

	EQUATION("productivity_c_new_max")
		/*UNIT: number of capital goods
		
		Max productivity in the C goods sector for new capital goods */

		v[0] = MAX("_productivity_c_new");

	RESULT(v[0])

	EQUATION("productivity_c_new_min")
		/*UNIT: number of capital goods
		
		Min productivity in the C goods sector for new capital goods */

		v[0] = MIN("_productivity_c_new");

	RESULT(v[0])

	EQUATION("green_productivity_c_new_max")
		/*UNIT: number of capital goods
		
		Max productivity in the C goods sector for new capital goods */

		v[0] = MAX("_green_productivity_c_new");

	RESULT(v[0])		

	EQUATION("green_productivity_c_new_avg")
		/*UNIT: number of emissions per consumption good produced
		
		Average green productivity in the C goods sector for new capital goods */

		v[0] = AVE("_green_productivity_c_new");

	RESULT(v[0])

//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 2: PRODUCTION AGGREGATES----------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("production_desired_k")
		/*UNIT: number of capital goods
		
		Aggregate desired prodcution in the K goods sector*/

		v[0] = SUM("_production_desired_k");

	RESULT(v[0])


	EQUATION("production_k")
		/*UNIT: number of capital goods
		
		Aggregate effective prodcution in the K goods sector*/

		v[0] = SUM("_production_k");

	RESULT(v[0])


	EQUATION("production_canceled_k")
		/*UNIT: number of capital goods
		
		Aggregate effective prodcution in the K goods sector*/

		v[0] = SUM("_production_canceled_k");

	RESULT(v[0])	
	
	
	EQUATION("clients_new_k")
		/*UNIT: number of consumption firms (clients)
		
		Sum of the new clients foe each K firm*/

		v[0] = SUM("_clients_new_k");


	RESULT(v[0])

	EQUATION("total_k_firms")
		/*UNIT: number of firms
		
		Number of firms in K sector*/
		
		v[0] = 0;

		CYCLE(cur, "k_firms")
			v[0]++

	RESULT(v[0])


//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------- PART 3: LABOR MARKET AGGREGATES ---------------------------------------------------------------------------------------------------===================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("researchers_k_demand")
		/*UNIT: number of researchers
		
		Aggregate demand for researchers in K sector */

		v[0] = SUM("_researchers_k_demand");

	RESULT(v[0])	


	EQUATION("workers_k_demand")
		/*UNIT: number of workers
		
		Aggregate demand for workers in K sector */

		v[0] = SUM("_workers_k_demand");

	RESULT(v[0])

	EQUATION("managers_k_demand")
		/*UNIT: number of workers
		
		Aggregate demand for managers in K sector */

		v[0] = SUM("_managers_k_demand");

	RESULT(v[0])


	EQUATION("researchers_k_employed")
		/*UNIT: number of researchers
		
		Aggregate demand for researchers in K sector */

		v[0] = SUM("_researchers_k_employed");

	RESULT(v[0])	


	EQUATION("workers_k_employed")
		/*UNIT: number of workers
		
		Aggregate demand for workers in K sector */

		v[0] = SUM("_workers_k_employed");

	RESULT(v[0])	

	EQUATION("managers_k_employed")
		/*UNIT: number of workers
		
		Aggregate demand for managers in K sector */

		v[0] = SUM("_managers_k_employed");

	RESULT(v[0])	


//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 4: MONETARY AGGREGATES ------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
		

	EQUATION("cost_unit_production_k_average")
		/*UNIT: monetary units
		
		 Average cost per unit produced in K sector*/

		v[0] = AVE("_cost_unit_production_k");

	RESULT(v[0])


	EQUATION("price_k_avg")
		/*UNIT: monetary units
		
		 Average price of capital goods*/

		v[0] = AVE("_price_k");

	RESULT(v[0])

	EQUATION("price_k_max")
		/*UNIT: monetary units
		
		 Average price of capital goods*/

		v[0] = MAX("_price_k");

	RESULT(v[0])


	EQUATION("cost_workers_k")
		/*UNIT: monetary units
		
		Aggregate cost with workers in K sector*/

		v[0] = SUM("_cost_workers_k");

	RESULT(v[0])

	EQUATION("cost_managers_k")
		/*UNIT: monetary units
		
		Aggregate cost with managers in K sector*/

		v[0] = SUM("_cost_managers_k");

	RESULT(v[0])	


	EQUATION("cost_unit_production_k_avg")
		/*UNIT: monetary units
		
		Aggregate cost with workers in C sector*/

		v[0] = SUM("_production_k") == 0? 0: SUM("_cost_production_k") / SUM("_production_k");

	RESULT(v[0])
	
	EQUATION("research_cost_k")
		/*UNIT: monetary units
		
		Aggregate cost with researchers in K sector*/

		v[0] = SUM("_research_cost_k");

	RESULT(v[0])		


	EQUATION("sales_k_nominal")
		/*UNIT: monetary units
		
		Aggregate sales in K sector*/

		v[0] = SUM("_sales_k_nominal");

	RESULT(v[0])

	EQUATION("taxes_sales_paid_k")
		/*UNIT: monetary units
		
		Aggregate sales taxes paid by K sector*/

		v[0] = SUM("_taxes_sales_paid_k");

	RESULT(v[0])

	EQUATION( "profits_distributed_k" )
		/*UNIT: monetary units

		Aggregate distributed profits from the K sector*/

		v[0] = SUM( "_profits_distributed_k");

	RESULT( v[0])	

	EQUATION( "profits_gross_k" )
		/*UNIT: monetary units

		Aggregate gross profits from the K sector*/

		v[0] = SUM( "_profits_gross_k");

	RESULT( v[0])			


	EQUATION("deposits_k")
		/*UNIT: monetary units
		
		Aggregate deposits in K sector*/

		v[0] = SUM("_deposits_k");

	RESULT(v[0])

	EQUATION("loan_k")
		/*UNIT: monetary units
		
		Aggregate deposits in K sector*/

		v[0] = SUM("_loan_k");

	RESULT(v[0])
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
// PART 4: ENVIRONMENTAL AGGREGATES ------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------===================================================================================================================================================

	EQUATION("emissions_k")
		/*UNIT:  tons of CO2 
		
		Aggregate emissions in K sector*/

		v[0] = SUM("_emissions_k");

	RESULT(v[0])

	EQUATION("green_productivity_k_average")
		/*UNIT:  tons of CO2 
		
		Aggregate emissions in K sector*/

		v[0] = V("production_k") == 0? 0: V("emissions_k") / V("production_k") ;

	RESULT(v[0])

	EQUATION( "green_k")
		/*UNIT: number of green firms
		
		Set the dummy for K firms green status (with lowest green emissions coef) 
		Counts the number of green firms (must be equal to green_treshold% of K firms) */

		SORT("k_firms", "_green_productivity_k", "UP"); // sorts the firms down (from more to less green)

		v[0] = COUNT("k_firms") * VS(PARENT, "green_treshold") ;
		v[1] = 0; 					//firms counter

		CYCLE(cur, "k_firms"){
			
			if( v[1] <= v[0] ){

				WRITES(cur, "_green_k", 1);    //write green dummy == 1

				v[1] ++;

			}
			else{

				WRITES(cur, "_green_k", 0);    //write green dummy == 0

			}
		}

	RESULT( v[1] )



