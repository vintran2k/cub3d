/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vintran <vintran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 17:43:36 by vintran           #+#    #+#             */
/*   Updated: 2021/03/24 22:47:20 by vintran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int		is_map_params(t_var *var)
{
	if (var->no == NULL || var->so == NULL ||
	var->we == NULL || var->ea == NULL ||
	var->sp == NULL || !var->rx || !var->ry)
		return (0);
	if (var->f == -1 || var->c == -1)
		return (0);
	return (1);
}

void	get_map_params(char *line, t_var *var)
{
	line += through_space(line);
	if (line[0] == 'S' && line[1] == 'O')
		get_path_texture(var, line, &var->so);
	else if (line[0] == 'N' && line[1] == 'O')
		get_path_texture(var, line, &var->no);
	else if (line[0] == 'E' && line[1] == 'A')
		get_path_texture(var, line, &var->ea);
	else if (line[0] == 'W' && line[1] == 'E')
		get_path_texture(var, line, &var->we);
	else if (line[0] == 'S' && (line[1] == ' ' || line[1] == '.'))
		get_path_texture(var, line, &var->sp);
	else if (line[0] == 'R' && (line[1] == ' ' || ft_isdigit(line[1])))
		get_resolution(var, line);
	else if (line[0] == 'C' && (line[1] == ' ' || ft_isdigit(line[1])))
		get_rgb(var, line, &var->c);
	else if (line[0] == 'F' && (line[1] == ' ' || ft_isdigit(line[1])))
		get_rgb(var, line, &var->f);
	else if (!is_empty(line))
		ft_error(var, "Invalid precision in .cub\n");
}

void	get_map_size(t_var *var)
{
	static int empty_before;

	if (!check_map_char(var->line))
		ft_error(var, "Invalid precision in .cub\n");
	if (empty_before == 1 && !is_empty(var->line))
		ft_error(var, "Empty line in the map\n");
	if (is_empty(var->line))
		empty_before = 1;
	else
	{
		var->l_line = is_longest(var->line, var->l_line);
		var->map_lines++;
		empty_before = 0;
	}
}

void	parsing_file(char *file, t_var *var)
{
	int		fd;
	int		ret;

	ret = 1;
	if ((fd = open(file, O_DIRECTORY)) != -1)
		ft_error(var, ".cub is a directory\n");
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_error(var, "open() file .cub failled\n");
	while (ret)
	{
		ret = get_next_line(fd, &var->line, var);
		if (!is_map_params(var))
		{
			get_map_params(var->line, var);
			var->map_beg++;
		}
		else if (is_empty(var->line) && var->l_line == 0)
			var->map_beg++;
		else if (!(is_empty(var->line) && var->l_line == 0))
			get_map_size(var);
		free(var->line);
		var->line = NULL;
	}
	close(fd);
	get_map(file, var);
}

void	parsing(int argc, char **argv, t_var *var)
{
	int i;

	init_var(var);
	if (argc < 2 || argc > 3)
		ft_error(var, "Incorrect number of arguments\n");
	if (argc == 3)
	{
		if (!ft_strcmp(argv[2], "--save"))
			var->save = 1;
		else
			ft_error(var, "Incorrect Arguments\n");
	}
	i = 0;
	while (argv[1][i])
		i++;
	while (i >= 0 && argv[1][i] != '.')
		i--;
	if (i == 0 || ft_strcmp(&argv[1][i], ".cub"))
		ft_error(var, "Incorrect Arguments\n");
	parsing_file(argv[1], var);
	parsing_map(var);
}
