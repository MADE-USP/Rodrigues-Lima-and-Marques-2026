
setwd("/Users/luccahenrique/LSD/Work/green_made/green_model_v3") # set working directory


library(ggplot2)
library(tidyverse)
library(mFilter)
library(ggcorrplot)
library(writexl)
library(MASS)  
library(knitr)
library(tidyr)
library(dplyr)
library(purrr)
library(boot)


#options:

baseName <- "purp1"                 # data files base name (same as .lsd file)
folder <- "purp1"                   # data files folder
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


# Se eu rodo primeiro no LSD depois rodo o R (precisa ser 50 simulações por causa do _[0-9])
# myFiles <- list.files(path = folder, pattern = paste0( baseName, "_[0-9]+.res" ),
#                        full.names = TRUE )

  # Define the folder name based on the experiment
  folderPath <- paste0(baseName)
  
  # List files within the specific experiment folder
  myFiles <- list.files(path = folderPath, 
                        pattern = paste0(baseName, "_[0-9]+\\.res"), 
                        full.names = TRUE)
  
  cat( "Data files: ", myFiles, "\n" )
  
  # Read data from text files and format it as a 3D array with labels
  mc <- read.4d.lsd( myFiles, microVars, skip = iniDrop, nrows = nKeep, nnodes = 1 )
  
  # Get dimensions of the array
  dim_names <- dimnames(mc)
  time_steps <- dim_names[[1]]  # 1st dim: time steps
  variables <- dim_names[[2]]   # 2nd dim: variable names
  instances <- dim_names[[3]]   # 3rd dim: instances
  mc_sims <- dim_names[[4]]     # 4th dim: Monte Carlo runs (to be aggregated)
  
  # Initialize a list to store dataframes (one per variable)
  variable_dfs <- list()
  
  # Loop over each variable
  for (var in variables) {
    # Extract data for the current variable (all time steps, instances, and MC sims)
    var_data <- mc[, var, , ]
    
    # Compute median across Monte Carlo simulations (4th dim) for each [time_step, instance]
    median_results <- apply(var_data, MARGIN = c(1, 2), median, na.rm = TRUE)
    
    # Convert to a dataframe: rows = time steps, columns = instances
    df <- as.data.frame(median_results)
    colnames(df) <- instances  # Column names = instance IDs
    df$time_step <- as.numeric(time_steps) # Add time steps as a column
    
    # Store the dataframe in the list
    variable_dfs[[var]] <- df
  }
  
# 
#   df <- variable_dfs[["X_investment_rate_c"]][-(1:50), ]
#   
#   spike_data <- df %>%
#     pivot_longer(
#       cols = -time_step,
#       names_to = "instance",
#       values_to = "investment_rate"
#     ) %>%
#     mutate(
#       spike_type = case_when(
#         investment_rate > 0.2 ~ "High Spike (I/K > 0.2)",
#         investment_rate < 0.02 ~ "Low Spike (I/K < 0.02)",
#         TRUE ~ "No Spike"
#       )
#     )
#   
#   spike_summary <- spike_data %>%
#     group_by(time_step, spike_type) %>%
#     summarise(
#       n = n(),
#       .groups = "drop"
#     ) %>%
#     group_by(time_step) %>%
#     mutate(
#       total_instances = sum(n),
#       proportion = n / total_instances
#     ) %>%
#     filter(spike_type != "No Spike")  # Optional: exclude non-spikes
#   
#   ggplot(spike_summary, aes(x = time_step, y = proportion, color = spike_type)) +
#     geom_line(size = 1) +
#     scale_color_manual(
#       values = c("High Spike (I/K > 0.2)" = "red", "Low Spike (I/K < 0.02)" = "blue")
#     ) +
#     scale_y_continuous(labels = scales::percent) +
#     labs(
#       x = "Time Step",
#       y = "Proportion of Instances",
#       title = "Proportion of High/Low Investment Spikes (Median Across MC Sims)",
#       color = "Spike Type"
#     ) +
#     theme_minimal()

  ############
  
  raw_prod_data <- mc[, "X_green_productivity_c_new", , ]  # dim: time × instances × MC_sims
  
  # Reshape to tidy format
  prod_df <- as.data.frame.table(raw_prod_data, responseName = "productivity") %>%
    rename(
      time_step = Var1,
      instance = Var2,
      mc_sim = Var3
    ) %>%
    mutate(
      time_step = as.numeric(time_step),
      instance = as.character(instance)
    )
  
  prod_summary <- prod_df %>%
    group_by(time_step) %>%
    summarise(
      median = median(productivity, na.rm = TRUE),
      minimum = min(productivity, na.rm = TRUE),
      # Exact 95% CI from empirical distribution
      lower_ci = quantile(productivity, 0.05, na.rm = TRUE),
      upper_ci = quantile(productivity, 0.95, na.rm = TRUE),
      .groups = "drop"
    )
  
  
  ggplot(prod_summary, aes(x = time_step)) +
    geom_ribbon(
      aes(ymin = lower_ci, ymax = upper_ci),
      fill = "grey70", alpha = 0.3
    ) +
    geom_line(aes(y = median, color = "Median"), size = 1) +
    geom_line(aes(y = minimum, color = "Minimum"), size = 1) +
    scale_color_manual(
      name = "",
      values = c("Median" = "black", "Minimum" = "#45ff66")
    ) +
    labs(
      x = "Time Step",
      y = "Carbon intensity"
    )  +
    theme(panel.border = element_rect(color = "black", size = 0.5, fill = NA),
          panel.background = element_rect(fill = "white"),
          legend.key = element_blank(),
          legend.position = c(0.1, 0.85),
          legend.background = element_blank()) 
  
  # ggsave("ci_heterogenous.pdf", path = "plots_green_files/figure-latex/")
  
#######
  
  
load("baseline_micro.RData")

df_baseline_micro <- mc_df_baseline[-(1:50), ]

growth_long <-  df_baseline_micro %>%
  dplyr::select(cols = matches("^X_sales_c_real\\.purp1_")) %>%
  pivot_longer(
    cols = starts_with("cols"),
    values_to = "sales"
  ) %>%
  dplyr::filter(!is.na(sales))

sales_long <-  df_baseline_micro %>%
  dplyr::select(cols = matches("^X_sales_c_real\\.purp1_")) %>%
  pivot_longer(
    cols = starts_with("cols"),
    values_to = "sales"
  ) %>%
  dplyr::filter(!is.na(sales))

# Pool all sales, rank in descending order
ranked_sales <- sales_long %>%
  dplyr::filter(sales > 0) %>%
  arrange(desc(sales)) %>%
  mutate(rank = row_number())

# Fit log-normal
fit <- fitdistr(ranked_sales$sales, "lognormal")
meanlog <- fit$estimate[1]
sdlog   <- fit$estimate[2]

# Generate fitted curve
x_seq <- seq(min(ranked_sales$sales), max(ranked_sales$sales), length.out = 500)
cdf <- plnorm(x_seq, meanlog, sdlog)
rank_fit <- (1 - cdf) * nrow(ranked_sales)

# Create a data frame for the fitted curve
fit_data <- data.frame(sales = x_seq, rank = rank_fit)

# Plot with fit line using the new data frame
ggplot(ranked_sales, aes(x = sales, y = rank)) +
  geom_point(size = 0.7) +
  geom_line(data = fit_data, aes(x = sales, y = rank), color = "black") +
  scale_x_log10() +
  scale_y_log10() +
  labs(
    x = "Size",
    y = "Rank"
  ) +
  theme(panel.border = element_rect(color = "black", size = 0.5, fill = NA),
        panel.background = element_rect(fill = "white"),
        legend.key = element_blank(),
        legend.position = c(0.1, 0.85),
        legend.background = element_blank()) 


# ggsave("zipf.pdf", path = "plots_green_files/figure-latex/")

##################

# Reshape data: rows = time steps, columns = firms
investment_long <-df_baseline_micro %>%
  dplyr::select(matches("^X_investment_rate_c\\.purp1_")) %>%
  mutate(time_step = row_number()) %>%  # Time steps are row indices
  pivot_longer(
    cols = matches("^X_investment_rate_c\\.purp1_"),
    names_to = "firm",
    values_to = "investment_rate"
  )
investment_long$investment_rate <- investment_long$investment_rate

# Classify spikes *for each time step* (no firm-level aggregation)
spike_counts <- investment_long %>%
  mutate(
    spike_type = case_when(
      investment_rate > 0.2 ~ "High Spike (I/K > 0.2)",
      investment_rate < 0.02 ~ "Low Spike (I/K < 0.02)",
      TRUE ~ "Other"  # Optional: track intermediate values
    )
  ) %>%
  dplyr::filter(spike_type != "Other") %>%  # Remove if only plotting High/Low
  count(time_step, spike_type)      # Count firms per time step and spike type

spike_counts$n <- spike_counts$n / 50

ggplot(spike_counts, aes(x = time_step, y = n, color = spike_type)) +
  geom_line(size = 0.5) +
  scale_color_manual(
    values = c("High Spike (I/K > 0.2)" = "red", "Low Spike (I/K < 0.02)" = "blue")
  ) +
  labs(
    x = "Time Step",
    y = "Number of Firms",
    title = "Firms with High/Low Investment Spikes Over Time",
    color = "Spike Type"
  ) +
  theme_minimal()

