
# ==== Manual configuration
setwd("/Users/luccahenrique/LSD/Work/green_made/green_model_v3") # set working directory


#options:

baseName <- "purp"                 # data files base name (same as .lsd file)
nExp <- 1            # number of experiments
folder <- "purp"                   # data files folder
iniDrop <- 0                        # initial time steps to drop from analysis (0=none)
nKeep <- -1                          # number of time steps to keep (-1=all)

file <- baseName

# configurations for tables and reports to be included in paper => note that some adjustments may be needed in the "support-functions" file in case these values are altered
plotW <- 10         # plot window width
plotH <- 7          # plot window height
plotRows  <- 2      # number of plots per row in a page
plotCols  <- 2  	# number of plots per column in a page
lowP      <- 6      # bandpass filter minimum period
highP     <- 32     # bandpass filter maximum period
bpfK      <- 12     # bandpass filter order
lags      <- 5      # lags to analyze in correlation structure
WarmUp   <- 50     #number of periods to be disregarded from analysis

# ====  Aggregated variables to use

aggrVars <- c(
  "cpi", "damage_shock_productivity", "demand_c_nominal", "demand_c_real",
  "emissions", "emissions_coefficient", "gdp_growth", "gdp_nominal", "gdp_real",
  "gdp_real_demand_approach", "inflation", "loans_to_gdp", "productivity_avg",
  "productivity_change", "profits_gross_nominal", "wage_share", "nominal_gdp_growth",
  "profit_share", "consumption_growth", "investment_growth", "clients_new_k",
  "cost_managers_k", "cost_unit_production_k_avg", "cost_workers_k", "deposits_k",
  "emissions_k", "green_k", "green_productivity_c_new_avg", "green_productivity_k_avg",
  "managers_k_employed", "price_k_avg", "production_canceled_k", "production_k",
  "productivity_c_new_avg", "productivity_k_avg", "profits_distributed_k",
  "researchers_k_demand", "researchers_k_employed", "sales_k_nominal",
  "workers_k_demand", "workers_k_employed", "profits_gross_k",
  "cost_unit_production_k_average", "capacity_ut_normal", "capacity_ut_c", "capital_stock_c", 
  "emissions_c", "exit_c", "green_c", "green_productivity_c_avg", "investment_demand_c",
  "investment_realized_c", "loan_c", "loan_c_demand", "managers_c_employed",
  "price_c_average_consumer", "production_c", "production_c_real",
  "sales_c_nominal", "workers_c_demand", "workers_c_employed",
  "cost_unit_production_c_avg", "profits_gross_c", "investment_realized_net_c",
  "propensity_consume_income", "capitalists", "consumption_realized_hh",
  "demand_consumption_hh", "deposits_hh", "dividends", "dividends_taxes",
  "gini_hh", "income_disposable_hh", "income_total_hh", "labor_demand",
  "labor_employed", "labor_force", "labor_restriction", "labor_unemployed",
  "unemployment_rate", "wage", "wage_bill", "wage_min", "wage_unemployed",
  "wage_sensitivity", "dividends_wage_ratio", "wage_demand_growth",
  "wage_sensitivity_hh", "deposits_hh_over_gdp", "wealth_tax",
  "capitalists_income", "wealth_tax_to_gdp", "CO2_tax", "dividends_tax_rate",
  "bonds_flow", "bonds_stock", "bonds_stock_to_gdp", "consumption_realized_gov",
  "demand_consumption_government", "gov_result_nominal_to_gdp",
  "gov_result_primary_to_gdp", "government_spending_realized",
  "green_bonds_stock", "green_government_spending", "policy_interest_rate",
  "taxes_emissions", "taxes_revenue", "taxes_to_gdp", "gov_spending_to_gdp",
  "gov_consumption_to_gdp", "unemployment_benefit_to_gdp",
  "green_government_result", "green_government_result_to_gdp",
  "green_government_spending_to_gdp", "taxes_emissions_to_gdp",
  "green_lab_cost", "researchers_cost_g", "researchers_g_employed",
  "green_productivity_g", "green_price_subsidie", "green_price_subsidie_cost",
  "green_research_subsidie", "green_research_subsidie_cost",
  "green_basic_income", "green_basic_income_cost", "research_wage_g",
  "green_transfer_cost", "green_transfer", "total_bonds_stock",
  "green_public_procurement_c", "green_public_procurement_cost",
  "dividends_taxes_to_gdp", "wealth_tax_rate", "loans_paid_exited_firms",
  "loans_unpaid_exited_firms", "bank_debt_supply_max", "bank_gov_bonds",
  "bank_loans", "bank_net_worth", "bank_reserves_cb", "interest_rate_loan",
  "wage_taxes", "propensity_consume_wage", "propensity_consume_dividends"
)

microVars <- c("_green_productivity_c_new",
               "_green_productivity_k",
               "_market_share_k",
               "_price_k",
               "_productivity_adoption",
               "_productivity_c_new",
               "_productivity_k",
               "_wage_k",
               "_investment_rate_c",
               "_mark_up_c",
               "_market_share_c",
               "_price_c",
               "_sales_c_real",
               "_wage_c")



# ==== Process LSD result files ====


library( LSDinterface, verbose = FALSE, quietly = TRUE ) # Package with LSD interface functions
library(xtable) # Package for exporting tables to Latex
library(ggplot2)
library(tidyverse)
library(RColorBrewer)
library(signal)
library(mFilter)
library(dygraphs)


# Se eu rodo primeiro no LSD depois rodo o R (precisa ser 50 simulações por causa do _[0-9])
# myFiles <- list.files(path = folder, pattern = paste0( baseName, "_[0-9]+.res" ),
#                        full.names = TRUE )


readExp <- function(exper) {
  
  # Define the folder name based on the experiment
  folderPath <- paste0(baseName, exper)
  
  # List files within the specific experiment folder
  myFiles <- list.files(path = folderPath, 
                        pattern = paste0(baseName, exper, "_[0-9]+\\.res"), 
                        full.names = TRUE)
  
  cat( "Data files: ", myFiles, "\n" )
  
  # Read data from text files and format it as a 3D array with labels
  mc <- read.3d.lsd( myFiles, aggrVars, skip = iniDrop, nrows = nKeep, nnodes = 1 )
  
  # Get dimensions information
  nTsteps <- dim( mc )[ 1 ]              # number of time steps
  nVar <- dim( mc )[ 2 ]                 # number of variables
  nSize  <- dim( mc )[ 3 ]               # Monte Carlo sample size
  
  # Compute Monte Carlo averages and std. deviation and store in 2D arrrays
  stats <- info.stats.lsd( mc, median = TRUE )
  
  
  # Insert a t column
  t <- as.integer( rownames( stats$avg ) )      # t = time
  A <- as.data.frame( cbind( t, stats$avg ) )   # A = average
  S <- as.data.frame( cbind( t, stats$sd ) )    # S = standard deviation
  Med <- as.data.frame( cbind( t, stats$med ) ) # Med = median
  Mad <- as.data.frame( cbind( t, stats$mad ) ) # Mad = median deviation
  M <- as.data.frame( cbind( t, stats$max ) )   # M = maximum
  m <- as.data.frame( cbind( t, stats$min ) )   # m = minimum
  
  # ==== Combining all data frames into one (with modified column names) -----
  
  old_names <- colnames(A)  # Get the current column names of the dataframe
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_A")  # Generate the new column name
    colnames(A)[i] <- new_name  # Assign the new column name
  }
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_m")  # Generate the new column name
    colnames(m)[i] <- new_name  # Assign the new column name
  }
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_M")  # Generate the new column name
    colnames(M)[i] <- new_name  # Assign the new column name
  }
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_S")  # Generate the new column name
    colnames(S)[i] <- new_name  # Assign the new column name
  }
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_Med")  # Generate the new column name
    colnames(Med)[i] <- new_name  # Assign the new column name
  }
  for (i in 2:length(old_names)) {
    new_name <- paste0(old_names[i], "_Mad")  # Generate the new column name
    colnames(Mad)[i] <- new_name  # Assign the new column name
  }
  
  df <- list (A, m, M, S, Med, Mad) %>% reduce(full_join, by='t')
  df_effective <- df[-(1:WarmUp),]
  
  #Saving data (parte que edita para cada modelo)
  
  if(exper == 1){
    mc_df_baseline <- as.data.frame(mc)
    df_baseline <- df_effective
    save(df_baseline, mc_df_baseline, file = "baseline.RData")
  }
  if(exper == 2){
    mc_df_scenario2 <- as.data.frame(mc)
    df_scenario2 <- df_effective
    save(mc_df_scenario2, df_scenario2, file = "scenario2.RData")
  }
  if(exper == 3){
    mc_df_scenario3 <- as.data.frame(mc)
    df_scenario3 <- df_effective
    save(mc_df_scenario3, df_scenario3, file = "scenario3.RData")
  }
  if(exper == 4){
    mc_df_scenario4 <- as.data.frame(mc)
    df_scenario4 <- df_effective
    save(mc_df_scenario4, df_scenario4, file = "scenario4.RData")
  }
  
  if(exper == 5){
    mc_df_scenario5 <- as.data.frame(mc)
    df_scenario5 <- df_effective
    save(mc_df_scenario5, df_scenario5, file = "scenario5.RData")
  }
  
  if(exper == 6){
    mc_df_scenario6 <- as.data.frame(mc)
    df_scenario6 <- df_effective
    save(mc_df_scenario6, df_scenario6, file = "scenario6.RData")
  }
  
  if(exper == 7){
    mc_df_scenario7 <- as.data.frame(mc)
    df_scenario7 <- df_effective
    save(mc_df_scenario7, df_scenario7, file = "scenario7.RData")
  }
  
  if(exper == 8){
    mc_df_scenario8 <- as.data.frame(mc)
    df_scenario8 <- df_effective
    save(mc_df_scenario8, df_scenario8, file = "scenario8.RData")
  }
  
  if(exper == 9){
    mc_df_scenario9 <- as.data.frame(mc)
    df_scenario9 <- df_effective
    save(mc_df_scenario9, df_scenario9, file = "scenario9.RData")
  }
  
  if(exper == 10){
    mc_df_scenario10 <- as.data.frame(mc)
    df_scenario10 <- df_effective
    save(mc_df_scenario10, df_scenario10, file = "scenario10.RData")
  }
  
  if(exper == 11){
    mc_df_scenario11 <- as.data.frame(mc)
    df_scenario11 <- df_effective
    save(mc_df_scenario11, df_scenario11, file = "scenario11.RData")
  }
  
  return( 0 )
}

lapply( 1 , readExp )


# ==== Create BPF series

# Baseline cycles
#   1. filter the series usging bkfilter()
#   2. store as a.data.frame
#   3. combine each data in a aggregate data frame "cycles"

# gdp_bpf <- bkfilter(log(df_baseline$gdp_real_Med), pl = lowP, pu = highP, nfix = bpfK )
# cycles_gdp_bpf <- as.data.frame(gdp_bpf$cycle)
# 
# investment_bpf <- bkfilter(log(df_baseline$production_k_Med), pl = lowP, pu = highP, nfix = bpfK )
# cycles_investment_bpf <- as.data.frame(investment_bpf$cycle)
# 
# consumption_bpf <- bkfilter(log(df_baseline$production_c_real_Med), pl = lowP, pu = highP, nfix = bpfK )
# cycles_consumption_bpf <- as.data.frame(consumption_bpf$cycle)
# 
# cycles <- cbind(t[-(1:WarmUp)], cycles_gdp_bpf, cycles_investment_bpf, cycles_consumption_bpf)
# 
# colnames(cycles)[c(1,2,3,4)] <- c("t", "cycles_gdp_bpf", "cycles_investment_bpf","cycles_consumption_bpf")

#
# ==== Plot statistics (calibration) ------
# #

# pdf( paste0(baseName, "_mc_plots.pdf" ), width = plotW, height = plotH )
# par( mfrow = c ( plotRows, plotCols ) ) # define plots per page
# 
# ccf(cycles$cycles_gdp_bpf, cycles$cycles_investment_bpf, lag.max = 4, na.action = na.pass)
# 
# ccf(cycles$cycles_gdp_bpf, cycles$cycles_consumption_bpf, lag.max = 4, na.action = na.pass)
# 
# ccf(cycles$cycles_gdp_bpf, cycles$cycles_inflation_bpf, lag.max = 4, na.action = na.pass)
# 
# ggplot(cycles) +
#   geom_line(aes(x = t, y = cycles_gdp_bpf, color = "cycles_gdp_bpf"), na.rm = TRUE) +
#   geom_line(aes(x = t, y = cycles_investment_bpf, color = "cycles_investment_bpf"), na.rm = TRUE) +
#   geom_line(aes(x = t, y = cycles_consumption_bpf, color= "cycles_consumption_bpf"), na.rm = TRUE) +
#   theme(legend.position = "bottom") +
#   labs(x = "Periods", y= "", title = 'BPF Filtered Series')
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = log(gdp_real_Med) )) +
#   geom_line(aes(x = t, y = log(gdp_real_Med + gdp_real_Mad)  ), linetype = "dashed") +
#   geom_line(aes(x = t, y = log(gdp_real_Med - gdp_real_Mad)  ), linetype = "dashed")
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = gdp_real_Med ))+
#   geom_line(aes(x = t, y = production_k_Med)) +
#   geom_line(aes(x = t, y = production_c_real_Med)) +
#   scale_y_continuous(trans='log10')
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = capacity_ut_c_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = production_k_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = production_c_real_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = emissions_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = gdp_growth_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = wage_share_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = gini_hh_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = bonds_stock_to_gdp_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = gov_result_primary_to_gdp_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = gov_result_nominal_to_gdp_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = green_productivity_c_new_avg_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = green_productivity_k_avg_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = productivity_c_new_avg_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = productivity_k_avg_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = capital_stock_c_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = unemployment_rate_Med))
# 
# ggplot(df_baseline) +
#   geom_line(aes(x = t, y = bank_debt_supply_max_Med))
# 
# dev.off()
