/******************************************************************************

	C_SECTOR OBJECT EQUATIONS
	----------------------

	Written by Lucca Rodrigues, Made-USP

	Equations that are specific to the consumption-good sector objects in the Made-Green model
	are coded below.

 ******************************************************************************/

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 1: INVESTMENT AGGREGATES--------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION( "investment_demand_c" )
		/*UNIT: number of capital goods
		
		Number of capital good demand in aggregate*/

		v[0] = SUM( "_investment_demand_c");

	RESULT( v[0])

	EQUATION( "investment_desired_expansion_c" )
		/*UNIT: number of capital goods
		
		Number of capital good demand in aggregate*/

		v[0] = SUM( "_investment_desired_expansion_c");

	RESULT( v[0])

	EQUATION( "investment_desired_substitution_c" )
		/*UNIT: number of capital goods
		
		Number of capital good demand in aggregate*/

		v[0] = SUM( "_investment_desired_substitution_c");

	RESULT( v[0])


	EQUATION( "investment_realized_c" )
		/*UNIT: number of capital goods
		
		Number of capital goods sold in aggregate*/

		v[0] = SUM( "_investment_realized_c");

	RESULT( v[0])

		EQUATION( "investment_realized_net_c" )
		/*UNIT: number of capital goods
		
		Number of net capital goods sold in aggregate*/

		v[0] = SUM( "_investment_realized_net_c");

	RESULT( v[0])

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 2: C MARKET --------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================

	EQUATION( "workers_c_demand" )
		/*UNIT: number of workers

		Number of workers demand in the C sectors*/

		v[0] = SUM( "_workers_c_demand");

	RESULT( v[0])

	EQUATION( "managers_c_demand" )
		/*UNIT: number of workers

		Number of managers demand in the C sectors*/

		v[0] = SUM( "_managers_c_demand");

	RESULT( v[0])

	EQUATION( "workers_c_employed" )
		/*UNIT: number of workers

		Number of workers demand in the C sectors*/

		v[0] = SUM( "_workers_c_employed");

	RESULT( v[0])

	EQUATION( "managers_c_employed" )
		/*UNIT: number of workers

		Number of managers demand in the C sectors*/

		v[0] = SUM( "_managers_c_employed");

	RESULT( v[0])

	EQUATION( "production_c" )
		/*UNIT: number of cpnsumption goods
		
		Number of capital goods produced in aggregate*/

		v[0] = SUM( "_production_c");

	RESULT( v[0])	


	EQUATION("capacity_ut_c")
		/*UNIT: rate
		
		Capacity utilization in the C sector*/

		v[0] = V("production_c_real") / V("capital_stock_c");	

	RESULT(v[0])


	EQUATION("capital_stock_c")
		/*UNIT: number of capital goods
		
		Aggregate capital stock*/

		v[0] = SUM("_capital_stock_c");

	RESULT(v[0])		


	EQUATION("production_c_nominal") 
		/* UNIT: monetary unit

		Aggregate supply in nominal terms */

		v[10] = 0; // set counter of real production to zero
		v[11] = 0; // set counter of nominal production to zero

		CYCLE(cur, "c_firms")
		{
			v[0] = VS(cur, "_production_c"); // production in real terms
			v[1] = VS(cur, "_price_c"); // price level

			v[10] += v[0]; // updates aggregate real production
			v[11] += (v[0] * v[1]); // updates aggregate nominal production
		}

		WRITE("production_c_real", v[10]);

	RESULT( v[11] )	


	EQUATION( "sales_c_real" )
		/*UNIT: number of consumption goods
		
		Number of capital goods sold in aggregate*/

		v[0] = SUM( "_sales_c_real");

	RESULT( v[0])


	EQUATION("inventories_c_real")
		/* UNIT: number of goods

		Sum of C firms' inventories */
	RESULT( SUM("_inventories_c") )


	EQUATION("inventories_c_real_change")
		/* UNIT: number of goods

		Change in real inventories */

		v[0] = V("inventories_c_real");

		v[1] = VL("inventories_c_real", 1); 

		v[2] = v[0] - v[1];

	RESULT( v[2] )


	EQUATION("productivity_c_avg")
		/*UNIT: number of capital goods
		
		Average effective productivity in the C goods sector*/

		v[0] = V( "production_c") / V( "workers_c_employed");

	RESULT(v[0])


	EQUATION("workers_wage_c_avg")
		/*UNIT: monetary units
		
		Average effective productivity in the C goods sector*/

		v[0] = V( "cost_workers_c") / V( "workers_c_employed");

	RESULT(v[0])


	EQUATION("green_productivity_c_avg")
		/*UNIT: number of emissions per consumption good produced
		
		Average effective green productivity in the C goods sector */

		v[0] = V( "emissions_c") / V( "production_c") ;

	RESULT(v[0])


	EQUATION("competitiveness_average_c")
		/* UNIT: irrelevant

		Average competitiveness of C goods firms weighted by their previous market share  */

		v[0] = 0; // counter of competitiveness * market share
		v[1] = 0; // counter of market shares

		SUM("_competitiveness_c"); // guarantee that all firms will be consulted (in case of bug)

		CYCLE(cur, "c_firms")
		{
			v[10] = VLS(cur,"_market_share_c", 1);

			v[13] = VS(cur,"_competitiveness_c");

				v[0] += (v[13] * v[10]); // updates counter of competitiveness * market share
				v[1] += v[10]; // updates counter of market share
		}

	RESULT( v[0] / v[1] )	


	EQUATION("unfilled_demand_c_average")
		/* UNIT: number of goods

		Average unfilled demand in consumption goods market (in the previous period) */

		v[0] = 0; // counter of goods
		v[1] = 0; // counter of firms

		SUML("_unfilled_demand_c", 1); // guarantee that all firms will be consulted (to avoid bug)

		CYCLE(cur, "c_firms")
		{			

			v[13] = VLS(cur, "_unfilled_demand_c", 1);

				v[0] += v[13]; // updates counter of goods
				v[1] ++; // updates counter of firms

				if(v[1] == 1)
				{
					v[2] = v[13];
					v[3] = v[13];
				}
				else
				{
					if(v[13] > v[2]) v[2] = v[13]; // updates max
					if(v[13] < v[3]) v[3] = v[13]; // updates min
				}
		}

		WRITE("unfilled_demand_c_max", v[2]);
		WRITE("unfilled_demand_c_min", v[3]);

	RESULT( v[0] / v[1] )


	EQUATION("market_share_c_rescale")
		/* UNIT: irrelevant

		Equation that corrects the firms' market shares so that sum = 1. This occurs before the consumption goods market opens and only affects the domestic firms' market shares  */

		
		v[1] = SUM("_market_share_c_before_rescale"); // only C firms
		v[0] = 0; // counter initialization

		CYCLE(cur, "c_firms")
		{
			v[2] = VS(cur, "_market_share_c_before_rescale") * ( 1.000000000 ) / v[1]; // rescale to add up to 1
			WRITES(cur, "_market_share_c", v[2]);

			v[0] += v[2]; // updates counter
		}

	RESULT( v[0] )


	EQUATION("total_c_firms")
		/*UNIT: number of firms
		
		Number of firms in C sector*/
		
		v[0] = 0;

		CYCLE(cur, "c_firms")
			v[0]++

	RESULT(v[0])



// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 3: MONETARY AGGREGATES --------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
	
	EQUATION("cost_workers_c")
		/*UNIT: monetary units
		
		Aggregate cost with workers in C sector*/

		v[0] = SUM("_cost_workers_c");

	RESULT(v[0])	

	
	EQUATION("cost_unit_production_c_avg")
		/*UNIT: monetary units
		
		Aggregate cost with workers in C sector*/

		v[0] = SUM("_production_c") == 0? 0: SUM("_cost_production_c") / SUM("_production_c");

	RESULT(v[0])	

	EQUATION("cost_managers_c")
		/*UNIT: monetary units
		
		Aggregate cost with workers in C sector*/

		v[0] = SUM("_cost_managers_c");

	RESULT(v[0])


	EQUATION("deposits_c")
		/*UNIT: monetary units
		
		Aggregate deposits in C sector*/

		v[0] = SUM("_deposits_c");

	RESULT(v[0])


	EQUATION( "profits_distributed_c" )
		/*UNIT: monetary units

		Aggregate distributed profits from the C sector*/

		v[0] = SUM( "_profits_distributed_c");

	RESULT( v[0])	

	EQUATION( "profits_gross_c" )
		/*UNIT: monetary units

		Aggregate gross profits from the C sector*/

		v[0] = SUM( "_profits_gross_c");

	RESULT( v[0])	


	EQUATION("price_c_average_before_sales") 
		/* UNIT: monetary units

		Average price level of consumption goods before sales - weighted by current market shares. In an open economy, it also considers the price of foreign goods (which will be imported) */
		
		v[1] = V("market_share_c_rescale"); // market shares have to have been rescaled (in an open economy, this will also consider the foreign sector's market share)
		v[0] = WHTAVE("_price_c", "_market_share_c"); // sum of products of price times market share

	RESULT( v[0] / v[1] ) 


	EQUATION("price_c_average_consumer") 
		/* UNIT: monetary units

		Average price actually paid by consumer (after sales take place). This may be different from prices before sales because the composition of supply may be different from that of sales (some firms may not have enough supply to cover demanded goods). This also considers the imported goods by the households (which are equal to zero in case of a closed economy)  */

		v[0] = V("sales_c_real") ; // goods acquired by consumers
		v[1] = V("sales_c_nominal") ; //   nominal amount spent by consumers

	RESULT( v[1] / v[0] )	


	EQUATION("sales_c_nominal")
		/*UNIT: monetary units
		
		Aggregate sales in C sector*/

		v[0] = SUM("_sales_c_nominal");

	RESULT(v[0])	


	EQUATION("loan_c_demand")
		/*UNIT: monetary units
		
		Aggregate loans in C sector*/

		v[0] = SUM("_loan_c_demand");

	RESULT(v[0])


	EQUATION("loan_c")
		/*UNIT: monetary units
		
		Aggregate loans in C sector*/

		v[0] = SUM("_loan_c");

	RESULT(v[0])	


	EQUATION("interest_paid_c")
		/*UNIT: monetary units
		
		Aggregate interest payments in C sector*/

		v[0] = SUM("_cost_loan_interest_due");

	RESULT(v[0])

	EQUATION("cost_investment_nominal_c")
		/*UNIT: monetary units
		
		Aggregate cost of investment in C sector*/

		v[0] = SUM("_cost_investment_c");

	RESULT(v[0])

	EQUATION("price_c_average_before_ms") 
		/* UNIT: monetary units

		Average price level in the consumption goods market (only considering domestic firms). It is the value before the market shares are updated and is used to calculate firms' competitiveness levels. */

		v[0] = 0; // counter of price 
		v[1] = 0; // counter of firms

		SUM("_price_c"); // just to guarantee that all firms will be consulted (to solve bug)

		CYCLE(cur, "c_firms")
		{

			v[12] = VS(cur,"_price_c");

				v[0] += v[12]; // updates counter of price
				v[1] ++; // updates counter of firms
				
				if(v[1] == 1) // if it is the first firm to be consulted: updates min and max prices
				{
					v[2] = v[12];
					v[3] = v[12];
				}
				else
				{
					if(v[12] > v[2]) v[2] = v[12]; // updates max price
					if(v[12] < v[3]) v[3] = v[12]; // updates min price
				}
		}

		WRITE("price_c_max", v[2]);
		WRITE("price_c_min", v[3]);


	RESULT( v[0] / v[1] )		

// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// PART 4: ENVIRONMENT --------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// ------------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
	
	EQUATION( "emissions_c" )
		/*UNIT: tons of CO2
		
		Emissions from producing in the consumption goods sector*/

		v[0] = SUM( "_emissions_c");

	RESULT( v[0])	


	EQUATION( "green_c")
		/*UNIT: number of green firms
		
		Set the dummy for C firms green status (with lowest green emissions) 
		Counts the number of green firms (must be equal to green_treshold% of C firms) */

		SORT("c_firms", "_emissions_c", "UP"); // sorts the firms down (from more to less green)

		v[0] = COUNT("c_firms") * VS(PARENT, "green_treshold") ;
		v[1] = 0; 					//firms counter

		CYCLE(cur, "c_firms"){
			
			if( v[1] <= v[0] ){

				WRITES(cur, "_green_c", 1);    //write green dummy == 1

				v[1] ++;

			}
			else{

				WRITES(cur, "_green_c", 0);    //write green dummy == 0

			}
		}

	RESULT( v[1] )	


// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// DUMMY VARIABLES -------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================


	EQUATION_DUMMY("unfilled_demand_c_max", "unfilled_demand_c_average")  

	EQUATION_DUMMY("unfilled_demand_c_min", "unfilled_demand_c_average")  

	EQUATION_DUMMY("price_c_min", "price_c_average_before_ms")  

	EQUATION_DUMMY("price_c_max", "price_c_average_before_ms")  

	EQUATION_DUMMY("production_c_real", "production_c_nominal")



// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// EXIT ENTRY DYNAMIC -------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================
// -----------------------------------------------------------------------------------------------------------------------==========================================================================================================================================================================================================================================


	EQUATION("exit_c") 
		/* UNIT: number of firms

		Exit dynamics of consumption goods firms */

		//V("order_equations_c_sector"); // variables that need to be updated before exit/entry takes place

		VS(PARENT, "gdp_real");
			
		v[0] = 0; // counter of firms that exited the market
		v[1] = 0; // counter of loans unpaid by deleted firms
		v[2] = 0; // counter of loans paid by deleted firms 
		
		v[30] = 0; // counter of remained deposits by deleted firms 

		v[3] = V("exit_market_share_min_c"); // minimum market share so that the firm stays in the market

		v[7] = VS(PARENT, "capitalists_per_firm");

		v[8] = V( "market_share_c_rescale" ) ; // if open economy, considers only market share of domestic market 

		v[9] = 0; // counter of lost inventories
		v[10] = 0; // counter of established firms in period
/*
		CYCLE_SAFE(cur,"c_firms") 
		{
			v[11] = VS(cur,"_market_share_c") / v[8]; // firm's market share (considers only the market share of the domestic market )

			v[17] = T - VS(cur, "_date_entry_c"); // firm's age
			
			if( v[17] > V( "entry_periods_safe_c") && COUNT("c_firms") > 1 ) // if it is the last firm in the market, it cannot be deleted
			{
				v[10] ++; 

				if( VS(cur, "_loan_constrained_c_dummy") == 1 && VS(cur, "_loan_new_c") == VS(cur, "_cost_loan_repayment_budget_c") && VS(cur, "_deposits_c") <= 0 )
				{
					v[18] = 1; // firm is removed from market due to being financially constrained

				}
				else
				{
					v[18] = 0;
				}

				if(v[11] < v[3] || VS(cur, "_capital_stock_c") == 0 || v[18] == 1) // if firm's market share is lower than minimum and it is an established firm
				{
					v[0] ++; // updates counter of deleted firms
					 
					v[20] = VS(cur,"_id_firm_c"); 
					v[21] = VS(cur,"_deposits_c");
					v[22] = VS(cur,"_loan_c"); 

					v[9] += VS(cur, "_inventories_c");
					
					v[31] = min(v[21], v[22]); // paid loan
					v[2] += v[31]; // updates counter of loans repaid

					v[32] = v[22] - v[31]; // unpaid loans
					v[1] += v[32];	// updates counter of unpaid loans

					v[30] += max(v[21] - v[22], 0); // net wealth of the firm (deposits)

					int sec = strcmp( NAMES( cur ), "c_firms" ) == 0 ? 0 : 1;

					object *cli;

					CYCLES( cur, cli, "broch" )	// leave counterpart lists
						DELETE( SHOOKS( cli ) );				// delete from counterpart list

					if ( sec == 1 )
					// update firm map before removing LSD object in consumption sector
						EXEC_EXTS( GRANDPARENTS( cur ), countryE, firmCmap, erase,
				   		( int ) VS( cur, "_id_firm_c" ) );

					DELETE(cur); // deletes the firm
				} // close if firm exited the market
			} // close if firm established
		} // close cycle

		if(v[0] > 0)
		{
			// bank variables update

				INCRS(financial1, "bank_loans", -(v[1] + v[2]) ); // loans are reduced by amount of loans of exited firms

				INCRS(financial1, "bank_deposits", -v[2]); // deposits are reduced by amount of deposits used to pay loan

				INCRS(financial1, "bank_net_worth", -v[1]); // bank's net worth is reduced by amount of unpaid loan

			// C sector variables update

				INCR( "loan_c", -(v[1] + v[2]));

				INCR( "deposits_c", - v[2]);

				INCR( "inventories_c_real", -v[9]);
		}	

		WRITES(financial1, "loans_unpaid_exited_firms", v[1]);
		WRITES(financial1, "loans_paid_exited_firms", v[2]); 
		WRITE("exit_inventories_destroyed", v[9]);
		WRITES(financial1,"deposits_exited_firms", v[30]); 
	*/
	
	RESULT( v[0] )


	EQUATION("entry_c") 
		/* UNIT: number of firms

		Entry of new firms (one to one replacement) */

		v[0] = V("exit_c"); // number of firms that exited the market

			if(COUNT("c_firms") == 0) PLOG("\n -> ERROR: No firm to copy in entry function");

		v[1] = 0; // counter of new firms

		v[2] = MAX("_id_firm_c"); // max id of consumption goods firm

		v[5] = VS(PARENT, "capitalists_per_firm");

		v[6] = 0; //counter capital 
		v[7] = 0; //counter loan

		// reference values for updating the variables (average of remaining firms)
		v[11] = AVE("_sales_c_real");
		v[12] = AVE("_price_c");
		v[13] = AVE("_capital_stock_c");
		v[14] = 0;
		v[15] = 0;
		v[16] = AVE("_market_share_c");

		while(v[1] < v[0]) // while the number of firms added is smaller than the number that exited
		{
			v[1] ++; // updates the number of firms added
			v[2] ++; // ID of the new firm

			cur = ADDOBJ("c_firms"); // adds a firm
			// updates the firm 

			// parameters (all parameters need to be updated, otherwise they are equal to zero)
					WRITES(cur,"_id_firm_c", v[2]); // updates the firm's ID

					WRITES(cur,"_date_entry_c", v[4]);


			// update hooks and descending objects (vintage and broch)

			DELETE( SEARCHS( cur, "vintage" ) );		// remove empty instances
			DELETE( SEARCHS( cur, "broch" ) );

			object *suppl, *vintage;

			suppl = set_supplier( cur );
			v[21] = VS(suppl, "_productivity_c_new"); // new productivity of the machines
			v[22] = VS(suppl, "_green_productivity_c_new"); //new emissions coefficient
			v[23] = VS(suppl, "_price_k"); //price supplier

			WRITELLS(cur, "_capital_stock_c", v[13], T, 1);

			v[6] += v[13]; 

			vintage = ADDOBJS(cur, "vintage"); // creates the object vintage

			WRITES(vintage, "__capital_vintage", v[13]);
			WRITELLS(vintage, "__capital_vintage", v[13], T, 1); // update total capital in the vintage (variable)

			WRITES(vintage, "__init_capital_vintage", v[13]);
			WRITELLS(vintage, "__init_capital_vintage", v[13], T, 1); // update total capital in the vintage (variable)

			WRITES(vintage, "__productivity_vintage", v[21]);
			WRITELLS(vintage, "__productivity_vintage", v[21], T, 1); // updates the productivity of the new capital good (parameter)

			WRITES(vintage, "__vintage_date", T);
			WRITELLS(vintage, "__vintage_date", -1, T, 1); // period in which the capital was acquired (parameter)

			WRITES(vintage, "__green_productivity_vintage", v[22]);
			WRITELLS(vintage, "__green_productivity_vintage", v[22], T, 1); //updates the emissions coefficient of the new capital good (parameter)

		// variables update 

			WRITES(cur, "_production_c", 0);
			WRITELLS(cur, "_production_c", 0, T, 1);          			//production is the capital stock used at the normal capacity ut

			WRITES(cur, "_inventories_c", 0);
			WRITELLS(cur, "_inventories_c", 0, T, 1); 			//no inventories 

			WRITES(cur, "_sales_c_real", 0);
			WRITELLS(cur, "_sales_c_real", 0 , T, 1);     	//sold what produced
			WRITELLS(cur, "_sales_c_real", 0 , T, 2);
			WRITELLS(cur, "_sales_c_real", 0 , T, 3);
			WRITELLS(cur, "_sales_c_real", 0 , T, 4);

			WRITES(cur, "_sales_expectations_c", v[11]);  

			WRITES(cur, "_demand_c_real", 0);
			WRITELLS(cur, "_demand_c_real", 0 , T, 1);     	//demand = production
			WRITELLS(cur, "_demand_c_real", 0 , T, 2);
			WRITELLS(cur, "_demand_c_real", 0 , T, 3);
			WRITELLS(cur, "_demand_c_real", 0 , T, 4);

			WRITES(cur, "_market_share_c", v[16]);
			WRITELLS(cur, "_market_share_c", v[16], T, 1);
			WRITELLS(cur, "_market_share_c", v[16], T, 2);

			WRITES(cur, "_unfilled_demand_c", 0);
			WRITELLS(cur, "_unfilled_demand_c", 0, T, 1);

			WRITES(cur, "_emissions_c", 0);
			WRITELLS(cur, "_emissions_c", 0, T, 1);

			WRITES(cur, "_cost_unit_production_c", 0);
			WRITELLS(cur, "_cost_unit_production_c", 0, T, 1);

			WRITES(cur, "_price_c", v[12]);
			WRITELLS(cur, "_price_c", v[12], T, 1);

			WRITES(cur, "_profits_net_c", 0);
			WRITELLS(cur, "_profits_net_c", 0,  T, 1);
			WRITELLS(cur, "_profits_net_c", 0,  T, 2);
			WRITELLS(cur, "_profits_net_c", 0,  T, 3);
			WRITELLS(cur, "_profits_net_c", 0,  T, 4);

			WRITES(cur, "_deposits_c", 0);
			WRITELLS(cur, "_deposits_c", 0, T, 1);
		
			WRITES(cur, "_loan_c", v[13] * v[23] );
			WRITELLS(cur, "_loan_c", v[13] * v[23], T, 1);						//firms start with debt to cover average capital

			v[7] += v[13] * v[23];

			WRITES(cur, "_mark_up_desired_c",  v[14]);
			WRITELLS(cur, "_mark_up_desired_c", v[14], T, 1);
		
			WRITES(cur, "_mark_up_c",  v[14]);
			WRITELLS(cur, "_mark_up_c", v[14], T, 1);  

			WRITES(cur, "_workers_c_employed", 0);
			WRITELLS(cur, "_workers_c_employed", 0, T, 1);
			
		
		}	 // closes while -> all new firms have been created and parameters and variables have been updated

		INCR( "loan_c", v[7] );

		INCR( "capital_stock_c", v[6] );

	RESULT( v[1] )



